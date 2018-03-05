# from . import AssetImporter

from . import FBXImporter, AssetDataBase
from FishEngine import Vector2, Vector3, Quaternion, GameObject, Transform, RectTransform, Camera, Light, MeshRenderer, MeshFilter, Material, Mesh, Debug, Prefab, Object, SceneManager
# from FishEngine.UI import Text
import FishEngine
import os
import yaml

from . import YAMLUtils


def MakeQuat(d):
    return Quaternion(d['x'], d['y'], d['z'], d['w'])

def MakeVec2(d):
    return Vector2(d['x'], d['y'])

def MakeVec3(d):
    return Vector3(d['x'], d['y'], d['z'])

def MakeComponent(ctype:str, d:int):
    comp = None
    if ctype == 'Camera':
        comp = Camera()
        comp.orthographic = (d['orthographic']==1)
        comp.orthographicSize = d['orthographic size']
        comp.nearClipPlane = d['near clip plane']
        comp.farClipPlane = d['far clip plane']
    elif ctype == 'Light':
        comp = Light()
    elif ctype == 'MeshRenderer':
        # print('Add Meshrenderer')
        comp = MeshRenderer()
        m_Materials = d['m_Materials']
        if len(m_Materials) > 1:
            print("materials > 1")
        if m_Materials[0]['fileID'] == 0:
            # do not have materials
            pass
        else:
            comp.material = Material.defaultMaterial()
    elif ctype == 'MeshFilter':
        comp = MeshFilter()
        mesh_d = d['m_Mesh']
        guid = mesh_d['guid']
        if guid == '0000000000000000e000000000000000':
            id2mesh = {10202:'Cube', 10206:'Cylinder', 10207:'Sphere', 10208:'Capsule', 10209:'Plane', 10210:'Quad'}
            comp.mesh = Mesh.GetInternalMesh(id2mesh[mesh_d['fileID']])
        else:
            importer:FBXImporter = AssetDataBase.GUIDToImporter(guid)
            nodeName = importer.fileIDToRecycleName[mesh_d['fileID']]
            comp.mesh = importer.GetMeshByName(nodeName)
    elif ctype == 'MonoBehaviour':
        if d['m_Script']['guid'] == 'f70555f144d8491a825f0804e09c671c' and d['m_Script']['fileID'] == 708705254:
            # print("Add Text")
            comp = FishEngine.UI.Text()
            comp.text = d['m_Text']
    else:
        # print('Unkown component type:', ctype)
        pass
    return comp


def MakeGameObject(d:dict, fileID2Dict:dict, fileID2Object:dict)->GameObject:
    tid = d['m_Component'][0]['component']['fileID']
    # fileID2Object[tid] = go.transform
    tdict = fileID2Dict[tid]
    go_name = str(d['m_Name'])
    if tdict['name'] == 'Transform':
        t = fileID2Dict[tid]['Transform']
        go = GameObject(go_name)
    elif tdict['name'] == 'RectTransform':
        t = fileID2Dict[tid]['RectTransform']
        go = GameObject.CreateWithRectTransform(go_name)
        rt = go.components[1]
        rt.anchorMin = MakeVec2(t['m_AnchorMin'])
        rt.anchorMax = MakeVec2(t['m_AnchorMax'])
        rt.anchoredPosition = MakeVec2(t['m_AnchoredPosition'])
        rt.sizeDelta = MakeVec2(t['m_SizeDelta'])
        rt.pivot = MakeVec2(t['m_Pivot'])
    else:
        Debug.Log('[TODO]', tdict['name'])
        print(tdict)
        return None

    go.m_IsActive = (d['m_IsActive']==1)
    go.transform.localRotation = MakeQuat(t['m_LocalRotation'])
    go.transform.localPosition = MakeVec3(t['m_LocalPosition'])
    go.transform.localScale = MakeVec3(t['m_LocalScale'])
    go.transform.m_RootOrder = t['m_RootOrder']
    for c in d['m_Component'][1:]:
        cid = c['component']['fileID']
        d2 = fileID2Dict[cid]
        ctype = d2['name']
        comp = MakeComponent(ctype, d2[ctype])
        if comp is not None:
            go.AddComponent(comp)
            fileID2Object[cid] = comp
    return go


class UnityPrefabImporter:
    def __init__(self, filepath:str):
        self.__filepath = filepath
        with open(filepath+'.meta', 'r') as f:
            data = yaml.load(f)
            self.__guid =  data['guid']

        f, fileID2Index = YAMLUtils.removeUnityTagAlias(filepath)
        data = list(yaml.load_all(f))
        for fileID, line in fileID2Index.items():
            d = data[line]
            d['name'] = list(d.keys())[0]
            d['fileID'] = fileID
        self.__data = data

    @property
    def guid(self)->str:
        return self.__guid

    @staticmethod
    def __Load(data:dict)->Prefab:
        fileID2Dict = {}
        for d in data:
            fileID2Dict[d['fileID']] = d

        fileID2Object = {}

        def filter_(*name):
            return [(d[d['name']],d['fileID']) for d in data if d['name'] in name]

        # crate GameObject
        for d, fileID in filter_('GameObject'):
            go = MakeGameObject(d, fileID2Dict, fileID2Object)
            fileID2Object[fileID] = go
        
        transforms_d = filter_('Transform', 'RectTransform')

        # connect GameObject and Trnasform
        for d, fileID in transforms_d:
            go = fileID2Object[d['m_GameObject']['fileID']]
            fileID2Object[fileID] = go.transform

        # set parent
        for d, fileID in transforms_d:
            t = fileID2Object[fileID]
            for c in d['m_Children']:
                cid = c['fileID']
                fileID2Object[cid].SetParent(t, False)

        rootID = data[0]['Prefab']['m_RootGameObject']['fileID']
        rootGO = fileID2Object[rootID]
        prefab = Prefab()
        rootGO.m_PrefabInternal = prefab
        prefab.m_RootGameObject = rootGO
        prefab.m_fileIDToObject = fileID2Object
        return prefab

    def CreateNew(self, modifications:dict = None)->GameObject:
        """ modifications is a dict create from yaml:
            - target: {fileID: 4061066396226068, guid: 9d6d3eb0aeb78472982f0dee59c87319, type: 2}
                propertyPath: m_LocalPosition.x
                value: 3
                objectReference: {fileID: 0}
            - target: {fileID: 4061066396226068, guid: 9d6d3eb0aeb78472982f0dee59c87319, type: 2}
                propertyPath: m_LocalPosition.y
                value: 0
                objectReference: {fileID: 0}
        """
        import copy
        data = copy.deepcopy(self.__data)
        fileID2Dict = {}
        for d in data:
            fileID2Dict[d['fileID']] = d[d['name']]

        if modifications is not None:
            for m in modifications:
                assert(m['target']['guid'] == self.__guid)
                fileID = m['target']['fileID']
                if m['objectReference']['fileID'] != 0:
                    #TODO
                    # print('[TODO] this modification to Prefab is set by object:')
                    # print(m)
                    pass
                else:   # modified by value
                    pp = m['propertyPath']
                    pps = pp.split('.')
                    value = m['value']
                    d = fileID2Dict[fileID]
                    assert(fileID in fileID2Dict)
                    for p in pps[:-1]:
                        d = d[p]
                    d[pps[-1]] = value
        prefab = UnityPrefabImporter.__Load(data)
        rootGO = prefab.m_RootGameObject

        if modifications is not None:
            for m in modifications:
                assert(m['target']['guid'] == self.__guid)
                fileID = m['target']['fileID']
                if m['objectReference']['fileID'] != 0:
                    #TODO
                    pp = m['propertyPath']
                    if pp == 'm_Mesh':
                        target:MeshFilter = prefab.m_fileIDToObject.get(fileID, None)
                        guid = m['objectReference']['guid']
                        importer:FBXImporter = AssetDataBase.GUIDToImporter(guid)
                        meshName = importer.fileIDToRecycleName[m['objectReference']['fileID']]
                        mesh = importer.GetMeshByName(meshName)
                        print(target, mesh)
                        target.mesh = mesh
                    else:
                        print('[TODO] this modification to Prefab is set by object:')
                        print(m)
        return rootGO

class UnityProjectImporter:
    def __init__(self, path:str):
        super().__init__()
        self.__path = path          # eg./Users/yushroom/program/Unity/MonumentVally/Assets

        for root, _, files in os.walk(path):
            for file in files:
                ext = file.split('.')[-1].lower()
                fullpath = os.path.join(root, file)
                if ext == 'fbx':
                    importer = FBXImporter()
                    print("[TODO] FBXImporter.globalScale")
                    importer.globalScale = 100
                    importer.Import(fullpath)
                elif ext == 'prefab':
                    importer = UnityPrefabImporter(fullpath)
                else:
                    continue
                # elif ext == 'unity':
                #     print(file)
                guid = importer.guid
                AssetDataBase.s_GUIDToAssetPath[guid] = fullpath
                AssetDataBase.s_assetPathTOGUID[fullpath] = guid
                AssetDataBase.s_GUIDToImporter[guid] = importer

    # def GetAtPath(self, path:str):
    #     guid = AssetDataBase.AssetPathToGUID(path)
    #     return self.__guidToImporter[guid]

    # def GetImpoerterByGUID(self, guid:str):
    #     return self.__guidToImporter[guid]

class UnitySceneImporter:
    def __init__(self, path:str):
        f, fileID2Index = YAMLUtils.removeUnityTagAlias(path)
        data = list(yaml.load_all(f))
        for fileID, line in fileID2Index.items():
            d = data[line]
            d['name'] = list(d.keys())[0]
            d['fileID'] = fileID

        fileID2Dict = {}
        for d in data:
            fileID2Dict[d['fileID']] = d

        fileID2Object = {}

        def filter_(*name):
            return [(d[d['name']],d['fileID']) for d in data if d['name'] in name]

        prefab_d = filter_('Prefab')

        # crate GameObject
        for d, fileID in prefab_d:
            guid = d['m_ParentPrefab']['guid']
            mods = d['m_Modification']['m_Modifications']
            importer:UnityPrefabImporter = AssetDataBase.GUIDToImporter(guid)
            go = importer.CreateNew(mods)
            fileID2Object[fileID] = go

        for d, fileID in filter_('GameObject'):
            go = None
            prefabFileID = d['m_PrefabInternal']['fileID']
            if prefabFileID == 0:
                go = MakeGameObject(d, fileID2Dict, fileID2Object)
            else:
                # this go is just a reference in a prefab, do not Instantiate
                # print(d)
                pass
            if go is not None:
                fileID2Object[fileID] = go


        transform_d = filter_('Transform', 'RectTransform')

        # connect GameObject and Trnasform
        for d, fileID in transform_d:
            if 'm_GameObject' in d:
                go:GameObject = fileID2Object[d['m_GameObject']['fileID']]
                fileID2Object[fileID] = go.transform
                go.transform.m_RootOrder = d['m_RootOrder']
            else:
                # print('------->find transfrom inside a prefab')
                prefab_fileID = d['m_PrefabInternal']['fileID']
                inside_ID = d['m_PrefabParentObject']['fileID']
                prefab_go:GameObject = fileID2Object[prefab_fileID]
                prefab:Prefab = prefab_go.m_PrefabInternal
                t:Transform = prefab.FindObjectByFileID(inside_ID)
                fileID2Object[fileID] = t
                # print(fileID, prefab_fileID, inside_ID)

        # set parent
        for d, fileID in transform_d:
            t = fileID2Object[fileID]
            if 'm_Children' in d:
                for c in d['m_Children']:
                    cid = c['fileID']
                    fileID2Object[cid].SetParent(t, False)
        
        # parent is inside a prefab
        for d, fileID in transform_d:
            t:Transform = fileID2Object[fileID]
            if t.parent is None and d['m_PrefabInternal']['fileID'] == 0 and d['m_Father']['fileID'] != 0:
                # print(d)
                parent_fileID = d['m_Father']['fileID']
                parent_t:Transform = fileID2Object[parent_fileID]
                t.SetParent(parent_t, False)
        for d, fileID in prefab_d:
            m_TransformParent = d['m_Modification']['m_TransformParent']
            p_fileID = m_TransformParent['fileID']
            t:Transform = fileID2Object[fileID].transform
            if p_fileID != 0 and t.parent is None:
                t.SetParent(fileID2Object[p_fileID], False)

        # update rootOrder
        # for d, fileID in transform_d:
        #     t:Transform = fileID2Object[fileID]
        #     if t.m_RootOrder != t.GetSiblingIndex():
        #         # print(t, t.m_RootOrder)
        #         if t.parent is None:
        #             gos = SceneManager.GetActiveScene().GetRootGameObjects()
        #             children = [go.transform for go in gos]
        #             print([t.m_RootOrder for t in children])
        #         else:
        #             print([t.m_RootOrder for t in t.parent.children])
        #             print(sorted(t.parent.children, key=lambda t: t.m_RootOrder))
        #         # t.SetSiblingIndex(t.m_RootOrder)

        for d, fileID in transform_d:
            t:Transform = fileID2Object[fileID]
            rootOrders = [t.m_RootOrder for t in t.children]
            assert(rootOrders == sorted(rootOrders))    # TODO
            # print([t.m_RootOrder for t in t.children])

        gos = Object.FindObjectsOfType(GameObject)
        gos = [go for go in gos if go.transform.parent is None]
        gos = sorted(gos, key=lambda go: go.transform.m_RootOrder)
        print([go.name for go in gos])
        scene = SceneManager.GetActiveScene()
        scene.m_rootGameObjects = gos


        # PrintHierarchy([go.transform for go in scene.GetRootGameObjects()])
        gos = Object.FindObjectsOfType(GameObject)
        print("# of GameObjects:", len(gos))

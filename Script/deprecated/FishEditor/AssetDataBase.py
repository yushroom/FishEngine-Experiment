from enum import Enum, auto
from FishEngine import SceneManager, Scene, Object

import FishEditorInternal

class ImportAssetOptions(Enum):
    Default = 0
    ForceUpdate = 1
    ForceSynchronousImport = 8
    ImportRecursive = 0x100
    DontDownloadFromCacheServer = 0x2000
    ForceUncompressedImport = 0x4000


class AssetDataBase:
    s_assetPathTOGUID = {}
    s_GUIDToAssetPath = {}
    s_GUIDToImporter = {}
    s_InstanceIDToAssetPath = {}
    s_InstanceIDToGUIDAndFileID = {}

    @staticmethod
    def AssetPathToGUID(path:str)->str:
        return AssetDataBase.s_assetPathTOGUID[path]
        # return FishEditorInternal.AssetDataBase.AssetPathToGUID(path)

    @staticmethod
    def GUIDToAssetPath(guid:str)->str:
        return AssetDataBase.s_GUIDToAssetPath[guid]
        # return FishEditorInternal.AssetDataBase.GUIDToAssetPath(guid)

    @staticmethod
    def GUIDToImporter(guid:str)->str:
        if guid in AssetDataBase.s_GUIDToImporter:
            return AssetDataBase.s_GUIDToImporter[guid]

        from . import FBXImporter, UnityPrefabImporter
        fullpath = AssetDataBase.GUIDToAssetPath(guid)
        ext = fullpath.split('.')[-1].lower()
        if ext == 'fbx':
            print("imporing fbx:", fullpath)
            importer = FBXImporter()
            print("[TODO] FBXImporter.globalScale")
            editorScene = SceneManager.CreateScene("Editor")
            old = SceneManager.GetActiveScene()
            SceneManager.SetActiveScene(editorScene)
            # importer.globalScale = 100
            importer.Import(fullpath)
            SceneManager.SetActiveScene(old)
        elif ext == 'prefab':
            print("imporing prefab:", fullpath)
            importer = UnityPrefabImporter(fullpath)
        AssetDataBase.s_GUIDToImporter[guid] = importer
        return importer

    @staticmethod
    def GetAssetPath(asset: Object)->str:
        return AssetDataBase.s_InstanceIDToAssetPath[asset.GetInstanceID()]
        # return FishEditorInternal.AssetDataBase.GetAssetPathFromInstanceID(asset.GetInstanceID())

    @staticmethod
    def StaticInit():
        from FishEngine import MeshManager
        # internal meshes
        guid = '0000000000000000e000000000000000'
        name2fileID = {'Cube':10202, 'Cylinder':10206, 'Sphere':10207, 'Capsule':10208, 'Plane':10209, 'Quad':10210}
        for name, fileID in name2fileID.items():
            mesh = MeshManager.GetInternalMesh(name)
            # print(name,mesh.GetInstanceID(), fileID, guid)
            AssetDataBase.s_InstanceIDToGUIDAndFileID[mesh.GetInstanceID()] = (guid, fileID)

    @staticmethod
    def StaticClean():
        AssetDataBase.s_assetPathTOGUID.clear()
        AssetDataBase.s_GUIDToAssetPath.clear()
        AssetDataBase.s_GUIDToImporter.clear()
        AssetDataBase.s_InstanceIDToGUIDAndFileID.clear()

    @staticmethod
    def ImportAsset(path:str, options:ImportAssetOptions):
        # c++
        raise NotImplementedError

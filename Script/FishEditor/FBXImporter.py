import FishEditorInternal

# FBXImporter = FishEditorInternal.FBXImporter

from FishEngine import Object, Mesh, GameObject
import yaml

class FBXImporter:
    __slots__ = ('m_CachedPtr', 'guid', 'fileIDToRecycleName')
    def __init__(self):
        super().__init__()
        self.m_CachedPtr = FishEditorInternal.CreateFBXImporter()
        self.guid:int = None
        self.fileIDToRecycleName = None

    @property
    def cpp(self):
        return self.m_CachedPtr

    def Import(self, path:str):
        with open(path+'.meta') as f:
            meta = yaml.load(f)
        # print(meta)
        self.guid = meta['guid']
        meta = meta['ModelImporter']
        self.fileIDToRecycleName = meta['fileIDToRecycleName']
        # serializedVersion = meta['serializedVersion']
        meshes = meta['meshes']
        self.globalScale = meta['meshes']['globalScale']
        if 'useFileScale' in meshes:
            self.useFileScale = meta['meshes']['useFileScale']
        else:
            self.useFileScale = False

        self.cpp.assetPath = path
        self.cpp.Import()
        FishEditorInternal.AssetImporter.AddImporter(self.cpp, self.guid)

        from . import AssetDataBase
        # print(self.fileIDToRecycleName)
        for fileID, name in self.fileIDToRecycleName.items():
            self.cpp.UpdateFileID(fileID, name)
            obj = self.cpp.GetObjectByFileID(fileID)
            if obj is None:
                continue
            # print(fileID, name, obj.name)
            # assert(obj.name == name)
            AssetDataBase.s_InstanceIDToGUIDAndFileID[obj.GetInstanceID()] = (self.guid, fileID)

    def GetMeshByFileID(self, fileID:int)->Mesh:
        # print(fileID)
        return self.cpp.GetObjectByFileID(fileID)

    @property
    def globalScale(self)->float:
        return self.cpp.GetGlobalScale()
    @globalScale.setter
    def globalScale(self, value:float):
        self.cpp.SetGlobalScale(value)

    @property
    def useFileScale(self)->bool:
        return self.cpp.GetUseFileScale()
    @useFileScale.setter
    def useFileScale(self, value:bool):
        self.cpp.SetUseFileScale(value)

    @property
    def fileScale(self)->float:
        return self.cpp.GetFileScale()
    @fileScale.setter
    def fileScale(self, value:float):
        self.cpp.SetFileScale(value)
        

    def CreateNew(self):
        root = self.cpp.GetRootGameObject()
        newRoot = root.Clone()
        # go = GameObject("", cppObject=newRoot)
        return newRoot
        
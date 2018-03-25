import FishEditorInternal

# FBXImporter = FishEditorInternal.FBXImporter

from FishEngine import Object, Mesh, GameObject
import yaml

class FBXImporter(Object):
    __slots__ = ('guid', 'fileIDToRecycleName')
    def __init__(self):
        super().__init__()
        self.m_CachedPtr = FishEditorInternal.FBXImporter()
        self.guid:int = None
        self.fileIDToRecycleName = None

    def Import(self, path:str):
        with open(path+'.meta') as f:
            meta = yaml.load(f)
        # print(meta)
        self.guid = meta['guid']
        meta = meta['ModelImporter']
        l = meta['fileIDToRecycleName']
        self.fileIDToRecycleName = l
        serializedVersion = meta['serializedVersion']
        meshes = meta['meshes']
        self.globalScale = meta['meshes']['globalScale']
        if 'useFileScale' in meshes:
            self.useFileScale = meta['meshes']['useFileScale']
        else:
            self.useFileScale = False

        self.m_CachedPtr.Import(path)

    def GetMeshByFileID(self, fileID:int)->Mesh:
        print(fileID)
        m = self.m_CachedPtr.GetObjectByFileID(fileID)
        return Mesh.Wrap(m)

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
        go = GameObject("", cppObject=newRoot)
        return go
        
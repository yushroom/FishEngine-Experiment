import FishEditorInternal

# FBXImporter = FishEditorInternal.FBXImporter

from FishEngine import Object, Mesh
import yaml

class FBXImporter(Object):
    __slots__ = ('guid', 'fileIDToRecycleName')
    def __init__(self):
        super().__init__()
        self.m_CachedPtr = FishEditorInternal.FBXImporter()
        self.guid:int = None
        self.fileIDToRecycleName = None

    def Import(self, path:str):
        self.m_CachedPtr.Import(path)
        with open(path+'.meta') as f:
            meta = yaml.load(f)
        # print(meta)
        self.guid = meta['guid']
        l = meta['ModelImporter']['fileIDToRecycleName']
        self.fileIDToRecycleName = l

    def GetMeshByFileID(self, fileID:int)->Mesh:
        print(fileID)
        m = self.m_CachedPtr.GetObjectByFileID(fileID)
        return Mesh.Wrap(m)

    @property
    def globalScale(self)->float:
        return self.m_CachedPtr.GetGlobalScale()
    @globalScale.setter
    def globalScale(self, value:float):
        self.m_CachedPtr.SetGlobalScale(value)
        
from . import Object

class Prefab(Object):
    __slots__ = ('m_fileIDToObject', 'm_RootGameObject')
    def __init__(self):
        super().__init__()
        self.m_fileIDToObject = {}   # int->Object
        self.m_RootGameObject = None
       
    def FindObjectByFileID(self, fileID:int):
        if fileID not in self.m_fileIDToObject:
            print("ERROR: fileID not found:", fileID)
        return self.m_fileIDToObject[fileID]

    def Clean(self):
        del self.m_fileIDToObject
        del self.m_RootGameObject

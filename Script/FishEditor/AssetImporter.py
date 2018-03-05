class AssetImporter:
    def __init__(self):
       pass

    @staticmethod
    def Create(path:str)->'AssetImporter':
        raise NotImplementedError
        
    @property
    def assetPath(self)->str:
        pass

    def SaveAndReimport(self):
        pass

    @staticmethod
    def GetAtPath(path:str)->'AssetImporter':
        pass
class AssetImporter:
    def __init__(self):
       pass

    @staticmethod
    def Create(path:str)->'AssetImporter':
        raise NotImplementedError
        
    @property
    def assetPath(self)->str:
        raise NotImplementedError

    def SaveAndReimport(self):
        from . import AssetDataBase
        AssetDataBase.ImportAsset(self.assetPath)

    @staticmethod
    def GetAtPath(path:str)->'AssetImporter':
        raise NotImplementedError
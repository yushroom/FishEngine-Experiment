class AssetDataBase:
    s_assetPathTOGUID = {}
    s_GUIDToAssetPath = {}
    s_GUIDToImporter = {}

    @staticmethod
    def AssetPathToGUID(path:str)->str:
        return AssetDataBase.s_assetPathTOGUID[path]

    @staticmethod
    def GUIDToAssetPath(guid:str)->str:
        return AssetDataBase.s_GUIDToAssetPath[guid]

    @staticmethod
    def GUIDToImporter(guid:str)->str:
        return AssetDataBase.s_GUIDToImporter[guid]

    @staticmethod
    def StaticClean():
        AssetDataBase.s_assetPathTOGUID.clear()
        AssetDataBase.s_GUIDToAssetPath.clear()
        AssetDataBase.s_GUIDToImporter.clear()

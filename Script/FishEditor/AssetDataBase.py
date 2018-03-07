from enum import Enum, auto

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

    @staticmethod
    def AssetPathToGUID(path:str)->str:
        return AssetDataBase.s_assetPathTOGUID[path]

    @staticmethod
    def GUIDToAssetPath(guid:str)->str:
        return AssetDataBase.s_GUIDToAssetPath[guid]

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
            importer.globalScale = 1
            importer.Import(fullpath)
        elif ext == 'prefab':
            print("imporing prefab:", fullpath)
            importer = UnityPrefabImporter(fullpath)
        AssetDataBase.s_GUIDToImporter[guid] = importer
        return importer

    @staticmethod
    def StaticClean():
        AssetDataBase.s_assetPathTOGUID.clear()
        AssetDataBase.s_GUIDToAssetPath.clear()
        AssetDataBase.s_GUIDToImporter.clear()

    @staticmethod
    def ImportAsset(path:str, options:ImportAssetOptions):
        # c++
        raise NotImplementedError

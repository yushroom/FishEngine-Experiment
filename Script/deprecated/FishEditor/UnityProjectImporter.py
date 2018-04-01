from . import AssetDataBase
import yaml, os

class UnityProjectImporter:
    def __init__(self, path:str):
        super().__init__()
        self.__path = path          # eg./Users/yushroom/program/Unity/MonumentVally/Assets

        def getGUID(assetPath):
            metaPath = assetPath + '.meta'
            with open(metaPath) as f:
                meta = yaml.load(f)
            guid = meta['guid']
            return guid

        for root, _, files in os.walk(path):
            for file in files:
                ext = file.split('.')[-1].lower()
                fullpath = os.path.join(root, file)
                if ext == 'fbx':
                    pass
                    # print("imporing fbx:", fullpath)
                    # importer = FBXImporter()
                    # print("[TODO] FBXImporter.globalScale")
                    # importer.globalScale = 100
                    # importer.Import(fullpath)
                elif ext == 'prefab':
                    # print("imporing prefab:", fullpath)
                    pass
                    # importer = UnityPrefabImporter(fullpath)
                else:
                    continue
                # elif ext == 'unity':
                #     print(file)
                guid = getGUID(fullpath)
                # guid = importer.guid
                AssetDataBase.s_GUIDToAssetPath[guid] = fullpath
                AssetDataBase.s_assetPathTOGUID[fullpath] = guid
                # AssetDataBase.s_GUIDToImporter[guid] = importer

    # def GetAtPath(self, path:str):
    #     guid = AssetDataBase.AssetPathToGUID(path)
    #     return self.__guidToImporter[guid]

    # def GetImpoerterByGUID(self, guid:str):
    #     return self.__guidToImporter[guid]

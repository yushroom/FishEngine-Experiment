from collections import OrderedDict
import yaml
from FishEngine import Mesh, Object, Script, GameObject, Component

class SceneDumper:
    def __init__(self):
        self.todo = []
        self.done = set()
        self.dict = None
        # self.file = open(outputFilePath, 'w')

    # begin doc
    def begin(self):
        self.dict = OrderedDict()
    
    # end doc
    def end(self):
        # yaml.dump(self.dict, self.file)
        # self.dict = None
        pass

    def __pre(self, data):
        from . import AssetDataBase
        if data is None:
            return {'fileID': 0}
        if data.__class__ is list:
            return [self.__pre(x) for x in data]
        if data.__class__ is dict:
            return {a: self.__pre(b) for a, b in data.items()}
        if isinstance(data, Mesh):
            guid, fileID = AssetDataBase.s_InstanceIDToGUIDAndFileID[data.GetInstanceID()]
            return {'fileID': fileID, 'guid': guid}
        if isinstance(data, Object):
            self.__AddObject(data)
            fileID = data.localIdentifierInFile if data.localIdentifierInFile != 0 else data.instanceID
            return {'fileID': fileID}
        return data

    def d(self, label:str, data):
        assert(isinstance(label, str))
        self.dict[label] = self.__pre(data)

    def Dump(self, objs):
        with open('FishEngine_demo1.unity', 'w') as f:
            f.writelines([
                '%YAML 1.1\n',
                '%TAG !u! tag:unity3d.com,2011:\n'
            ])
            self.todo = objs[:]
            self.done = set(self.todo)
            
            while len(self.todo) > 0:
                o = self.todo.pop()
                self.begin()
                o.Serialize(self)
                self.end()
                fileID = o.localIdentifierInFile if o.localIdentifierInFile != 0 else o.instanceID
                f.write('--- !u!{} &{}\n'.format(o.ClassID, fileID))
                if isinstance(o, Script):
                    name = 'Script'
                else:
                    name = o.__class__.__name__
                yaml.dump({name: self.dict}, f)
                # yaml.dump({o.__class__.__name__: o.ToDict(self)}, f)
    
    def __AddObject(self, obj):
        assert(isinstance(obj, Object))
        if isinstance(obj, GameObject) or isinstance(obj, Component):
            prefab = obj.GetPrefabInternal()
            if prefab is not None:
                obj = prefab    # dump prefab instead
        if obj not in self.done:
            self.todo.append(obj)
            self.done.add(obj)
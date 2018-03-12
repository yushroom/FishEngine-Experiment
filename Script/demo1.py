from FishEngine import *
import yaml
from collections import OrderedDict

class Rotator2(Script):
    # __slots__ = ('speed', '__hidden')
    def __init__(self):
        super().__init__()
        self.speed = 1.0
        self.__hidden = True    # test

    # def SystemUpdate(self):
    def Update(self):
        # print(self.cpp.GetGameObject().name)
        self.transform.RotateAround(self.transform.parent.position, Vector3.up(), self.speed)
        # print(self.name, self.transform.localPosition, self.transform.position)
        # printT(self.transform)

# class Rotator2System(System):
#     def Update(self):
#         for go in GameObject.FindWithComponent(Rotator2):
#             go.GetComponent(Rotator2).SystemUpdate()

class Rotator3(Script):
    def __init__(self):
        super().__init__()
        self.speed = 1.0
        self.target = Vector3.zero()

    # def Update(self):
    #     self.transform.RotateAround(self.target, Vector3.up(), self.speed)
    #     self.transform.LookAt(self.target)

class Rotator3System(System):
    def Update(self):
        for go in GameObject.FindWithComponent(Rotator3):
            r = go.GetComponent(Rotator3)
            go.transform.RotateAround(r.target, Vector3.up(), r.speed)
            go.transform.LookAt(r.target)

def Vector3_representer(dumper, v:Vector3):
    return dumper.represent_dict(OrderedDict(x=v.x, y=v.y, z=v.z))

def Quaternion_representer(dumper, q:Quaternion):
    return dumper.represent_dict(OrderedDict(x=q.x, y=q.y, z=q.z, w=q.w))

def OrderedDict_representer(dumper, data):
    return dumper.represent_dict(data.items())

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
        if data is None:
            return {'fileID': 0}
        if data.__class__ is list:
            return [self.__pre(x) for x in data]
        if data.__class__ is dict:
            return {a: self.__pre(b) for a, b in data.items()}
        if isinstance(data, Object):
            self.__AddObject(data)
            return {'fileID': data.instanceID}
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
                f.write('--- !u!{} &{}\n'.format(o.ClassID, o.instanceID))
                yaml.dump({o.__class__.__name__: self.dict}, f)
                # yaml.dump({o.__class__.__name__: o.ToDict(self)}, f)
    
    def __AddObject(self, obj):
        assert(isinstance(obj, Object))
        if obj not in self.done:
            self.todo.append(obj)
            self.done.add(obj)


class SceneLoader:
    def __init__(self):
        pass
    
    def Load(self, path):
        with open(path, 'r') as f:
            pass

def Start():
    cameraGO = GameObject(name="Camera")
    camera = cameraGO.AddComponent(Camera())
    cameraGO.transform.position = Vector3(0, 1, -10)
    cameraGO.transform.position = Vector3(0, 5, -5)
    cameraGO.transform.LookAt(Vector3.zero())

    # Object.Instantiate(cameraGO)

    assert(cameraGO.cpp.GetPyObject() is cameraGO)
    yaml.add_representer(OrderedDict, OrderedDict_representer)
    # yaml.add_representer(GameObject, GameObject_representer)
    # yaml.add_representer(Transform, Transform_representer)
    yaml.add_representer(Vector3, Vector3_representer)
    yaml.add_representer(Quaternion, Quaternion_representer)

    lightGO = GameObject(name='Directional Light')
    lightGO.transform.localPosition = Vector3(0, 3, 0)
    lightGO.transform.localEulerAngles = Vector3(50, -30, 0)
    lightGO.AddComponent(Light())

    sun = GameObject.CreatePrimitive(PrimitiveType.Sphere)
    earth = GameObject.CreatePrimitive(PrimitiveType.Sphere)
    moon = GameObject.CreatePrimitive(PrimitiveType.Sphere)
    sun.name = 'Sun'
    earth.name = 'Earth'
    moon.name = 'Moon'

    earth.transform.position = Vector3(4, 0, 0)
    earth.transform.localScale = Vector3(0.5, 0.5, 0.5)
    moon.transform.localScale = Vector3(0.2, 0.2, 0.2)
    moon.transform.localPosition = Vector3(4.5, 0, 0)

    earth.transform.parent = sun.transform
    moon.transform.parent = earth.transform

    r:Rotator2 = earth.AddComponent(Rotator2())
    r.speed = 1
    moon.AddComponent(Rotator2()).speed = 2

    print(r.GetVisiableAttributes())

    plane = GameObject.CreatePrimitive(PrimitiveType.Plane)

    cameraGO.AddComponent(Rotator3())

    # scene = SceneManager.GetActiveScene()
    # scene.AddSystem(Rotator2System())

    objs = Object.FindObjectsOfType(GameObject)
    print(objs)

    dumper = SceneDumper()
    dumper.Dump(objs)

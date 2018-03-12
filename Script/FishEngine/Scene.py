from . import Object
from typing import List

import FishEngineInternal



class Scene:
    __slots__ = ('m_Handle', 'cpp')
    def __init__(self, cppScene=None):
        self.m_Handle:int = 0
        self.cpp = None

    # interal
    @property
    def gameObjects(self):
        raise NotImplementedError

    def Clean(self):
        self.cpp.Clean()

    @property
    def rootCount(self):
        raise NotImplementedError

    def GetRootGameObjects(self):
        raise NotImplementedError

    # @property
    # def systems(self):
    #     return self.__systems

    # def AddSystem(self, system):
    #     self.__systems.append(system)
    #     return system

    def Backup(self):
        pass

    def Restore(self):
        pass


class SceneBackupDumper:
    def __init__(self):
        self.todo = []
        self.done = set()
        self.dict = None
        self.objects = {}

    def __pre(self, data):
        from . import Mesh
        if data.__class__ is list:
            return [self.__pre(x) for x in data]
        if data.__class__ is dict:
            return {a: self.__pre(b) for a, b in data.items()}
        if isinstance(data, Mesh):
            guid = '0000000000000000e000000000000000'
            name2fileID = {'Cube':10202, 'Cylinder':10206, 'Sphere':10207, 'Capsule':10208, 'Plane':10209, 'Quad':10210}
            return {'fileID': name2fileID[data.name], 'guid':guid}
        if isinstance(data, Object):
            self.__AddObject(data)
            return {'fileID': data.instanceID}
        return data

    def d(self, label:str, data):
        assert(isinstance(label, str))
        self.dict[label] = self.__pre(data)
    
    def Dump(self, scene:Scene):
        self.todo = scene.GetRootGameObjects()
        # self.done 
        while len(self.todo) > 0:
            obj = self.todo.pop()
            instanceID = obj.instanceID
            if instanceID not in self.objects:
                self.dict = {}
                obj.Serialize(self)
                self.objects[instanceID] = self.dict

    def __AddObject(self, obj):
        assert(isinstance(obj, Object))
        if obj not in self.done:
            self.todo.append(obj)
            self.done.add(obj)

class SceneManager:
    __Handle2Scene = {}

    @staticmethod
    def CreateScene(sceneName: str):
        s = FishEngineInternal.SceneManager.CreateScene(sceneName)
        return SceneManager.__Wrap(s)

    @staticmethod
    def sceneCount():
        raise NotImplementedError

    @staticmethod
    def __Wrap(cppScene:FishEngineInternal.Scene)->Scene:
        handle = cppScene.GetHandle()
        if handle in SceneManager.__Handle2Scene:
            return SceneManager.__Handle2Scene[handle]
        else:
            s = Scene()
            s.cpp = cppScene
            SceneManager.__Handle2Scene[handle] = s
            return s

    @staticmethod
    def GetActiveScene()->Scene:
        cpp = FishEngineInternal.SceneManager.GetActiveScene()
        return SceneManager.__Wrap(cpp)

    @staticmethod
    def SetActiveScene(scene:Scene):
        FishEngineInternal.SceneManager.SetActiveScene(scene.cpp)

    # @staticmethod
    # def StaticClean():
    #     pass
from . import Object
from typing import List

import FishEngineInternal 

class Scene:
    __slot__ = ('m_Handle')
    def __init__(self):
        self.m_Handle:int = 0
        # self.__gameObjects = []
        self.__systems = [] # list of System
        # self.m_rootGameObjects = []

    # interal
    @property
    def gameObjects(self):
        # return self.__gameObjects
        raise NotImplementedError

    def Clean(self):
        # self.__gameObjects.clear()
        # self.m_rootGameObjects.clear()
        # raise NotImplementedError
        pass

    @property
    def rootCount(self):
        # return len(self.GetRootGameObjects())
        raise NotImplementedError

    def GetRootGameObjects(self):
        # return [o for o in self.__gameObjects if o.transform.parent is None]
        # return self.m_rootGameObjects
        raise NotImplementedError

    @property
    def systems(self):
        return self.__systems

    def AddSystem(self, system):
        self.__systems.append(system)
        return system



class SceneManager:
    __activeScene:Scene = None
    __scenes:List[Scene] = []

    @property
    def sceneCount(self):
        return len(self.__scenes)

    @staticmethod
    def GetActiveScene()->Scene:
        if SceneManager.__activeScene is None:
            s = Scene()
            SceneManager.__activeScene = s
            SceneManager.__scenes.append(s)
        return SceneManager.__activeScene

    @staticmethod
    def StaticClean():
        SceneManager.__activeScene = None
        SceneManager.__scenes = []
from . import Object
from typing import List

class Scene(Object):
    def __init__(self):
        super().__init__()
        self.__gameObjects = []
        self.__systems = [] # list of System
        # self.buildIndex
        # self.isDirty
        # self.isLoaded
        # self.name
        # self.path
        self.m_rootGameObjects = []

    # interal
    @property
    def gameObjects(self):
        return self.__gameObjects

    def Clean(self):
        self.__gameObjects.clear()
        self.m_rootGameObjects.clear()

    @property
    def rootCount(self):
        return len(self.GetRootGameObjects())

    def GetRootGameObjects(self):
        # return [o for o in self.__gameObjects if o.transform.parent is None]
        return self.m_rootGameObjects

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
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
        result = []
        FishEngineInternal.Scene_GetRootGameObjects(self.cpp, result)
        return result

    def Backup(self):
        pass

    def Restore(self):
        pass


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
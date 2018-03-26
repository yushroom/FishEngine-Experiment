from enum import Enum, auto
from FishEngine import SceneManager

class OpenSceneMode(Enum):
    '''Used when opening a scene in the Editor to specify how a scene should be opened.
    '''
    Single = auto
    Additive = auto
    AdditiveWithoutLoading = auto

class NewSceneSetup(Enum):
    '''Used when creating a new scene in the Editor.
    '''
    EmptyScene = auto           '''No game objects are added to the new scene.'''
    DefaultGameObjects = auto   '''Adds default game objects to the new scene (a light and camera).'''

class NewSceneMode(Enum):
    '''Used when creating a new scene in the Editor.
    '''
    Single = auto   '''All current open scenes are closed and the newly created scene are opened.'''
    Additive = auto '''The newly created scene is added to the current open scenes.'''


class EditorSceneManager:
    @staticmethod
    # def OpenScene(scenePath: str, mode: OpenSceneMode = OpenSceneMode.Single)->'Scene':
    def OpenScene(scenePath: str)->'Scene':
        '''Open a Scene in the Editor.
        '''
        pass

    @staticmethod
    def NewScene(setup: NewSceneSetup, mode: NewSceneMode = NewSceneMode.Single)->'Scene':
        pass

    @staticmethod
    def CloseScene(scene: 'SceneManagement.Scene', removeScene: bool) -> bool:
        pass
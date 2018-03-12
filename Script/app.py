import FishEngineInternal
from FishEngine import SceneManager, Material, Mesh
from timing import timing
import demo1, demo2

from FishEditor import AssetDataBase

import sys

def Start():
    scene = SceneManager.CreateScene("DefaultScene")
    SceneManager.SetActiveScene(scene)
    demo1.Start()

# @timing
def Update():
    # scene = SceneManager.GetActiveScene()
    # for s in scene.systems:
    #     s.Update()
    pass

def Backup():
    pass

def Restore():
    pass
    
def Clean():
    # SceneManager.GetActiveScene().Clean()
    # SceneManager.StaticClean()
    Material.StaticClean()
    Mesh.StaticClean()
    AssetDataBase.StaticClean()
    
def Reload():
    print('Reload from Python.....')
    import importlib, FishEngine
    importlib.reload(FishEngine)
    importlib.reload(demo2)
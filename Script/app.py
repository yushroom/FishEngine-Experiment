import FishEngineInternal
from FishEngine import SceneManager, Material, Mesh
from timing import timing
import demo1, demo2

from FishEditor import AssetDataBase

def Start():
    demo1.Start()
    # demo2.Start()

# @timing
def Update():
    scene = SceneManager.GetActiveScene()
    for s in scene.systems:
        s.Update()
    
def Clean():
    SceneManager.GetActiveScene().Clean()
    SceneManager.StaticClean()
    Material.StaticClean()
    Mesh.StaticClean()
    AssetDataBase.StaticClean()
    
def Reload():
    print('Reload from Python.....')
    import importlib, FishEngine
    importlib.reload(FishEngine)
    importlib.reload(demo2)
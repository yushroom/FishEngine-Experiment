import FishEngineInternal
from FishEngine import SceneManager, Material, MeshManager, Vector3, Quaternion, Script
from timing import timing
import demo1, demo2
from collections import OrderedDict

from FishEditor import EditorApplication

import sys, yaml

def Vector3_representer(dumper, v:Vector3):
    return dumper.represent_dict(OrderedDict(x=v.x, y=v.y, z=v.z))

def Quaternion_representer(dumper, q:Quaternion):
    return dumper.represent_dict(OrderedDict(x=q.x, y=q.y, z=q.z, w=q.w))

def OrderedDict_representer(dumper, data):
    return dumper.represent_dict(data.items())

def Start():
    # AssetDataBase.StaticInit()
    yaml.add_representer(OrderedDict, OrderedDict_representer)
    yaml.add_representer(Vector3, Vector3_representer)
    yaml.add_representer(Quaternion, Quaternion_representer)

    scene = SceneManager.CreateScene("DefaultScene")
    SceneManager.SetActiveScene(scene)

    demo2.Start()
    # project_path = r'D:\workspace\unity\FishEngine\Assets'
    # EditorApplication.OpenProject(project_path)
    # demo1.Start()

# @timing
def Update():
    pass
    
def Clean():
    # Material.StaticClean()
    MeshManager.StaticClean()
    Script.StaticClean()
    
def Reload():
    print('Reload from Python.....')
    import importlib, FishEngine
    importlib.reload(FishEngine)
    importlib.reload(demo2)

from FishEngine import GameObject, Object

# def Save():
#     objs = Object.FindObjectsOfType(GameObject)
#     dumper = SceneDumper()
#     dumper.Dump(objs)

#     scene = SceneManager.CreateScene("RuntimeScene")
#     SceneManager.SetActiveScene(scene)
#     scene_path = 'FishEngine_demo1.unity'
#     sceneImporter = UnitySceneImporter(scene_path)
#     sceneImporter.Import()
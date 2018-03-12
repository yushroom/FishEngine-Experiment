import yaml

from FishEngine import *

from timing import timing

def DefaultScene():
    camGO = GameObject('Main Camera')
    camGO.transform.localPosition = Vector3(0, 1, -10)
    camGO.AddComponent(Camera())
    lightGO = GameObject('Directional Light')
    lightGO.transform.localPosition = Vector3(0, 3, 0)
    lightGO.transform.localEulerAngles = Vector3(50, -30, 0)
    lightGO.AddComponent(Light())

from FishEditor import UnityProjectImporter, UnitySceneImporter, AssetDataBase
import os

@timing
def Start():
    # Screen.SetResolution(640//2, 1136//2, False)
    Screen.SetResolution(1136, 640, False)
    # project_path = '/Users/yushroom/program/Unity/MonumentVally/Assets'
    project_path = '/Users/yushroom/program/Unity/FishEngine/Assets'
    projectImporter = UnityProjectImporter(project_path)

    scene_path = os.path.join(project_path, "polygons.unity")
    sceneImporter = UnitySceneImporter(scene_path)
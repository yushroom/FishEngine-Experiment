import yaml

from FishEngine import *
from FishEditor import EditorApplication
import FishEditorInternal

from timing import timing
import platform

def DefaultScene():
    camGO = GameObject('Main Camera')
    camGO.transform.localPosition = Vector3(0, 1, -10)
    camGO.AddComponent(Camera())
    lightGO = GameObject('Directional Light')
    lightGO.transform.localPosition = Vector3(0, 3, 0)
    lightGO.transform.localEulerAngles = Vector3(50, -30, 0)
    lightGO.AddComponent(Light())

import os
import json

@timing
def Start():
    # Screen.SetResolution(640//2, 1136//2, False)
    Screen.SetResolution(1136, 640, False)
    if platform.system() == 'Windows':
        project_path = r'D:\workspace\unity\FishEngine'
        scene_path = 'Assets/TestPhysics.unity'
    else:
        project_path = r'/Users/yushroom/program/Unity/FishEngine'
        scene_path = 'Assets/TestPhysics.unity'
    EditorApplication.OpenProject(project_path)
    scene = FishEditorInternal.EditorSceneManager.OpenScene(scene_path, FishEditorInternal.OpenSceneMode.Single)
    FishEngineInternal.SceneManager.SetActiveScene(scene)

    # scenes = []
    # with open('scenes.json') as f:
    #     scenes = json.load(f)
    # # print(scenes)
    # last_scene = scenes[0]
    # project_path = last_scene['path']
    # scene_path = os.path.join(project_path, last_scene['lastOpenedScene'])
    # project_path = '/Users/yushroom/program/Unity/MonumentVally/Assets'
    # project_path = r'D:\program\github\MonumentVally-Demo\Assets'
    # projectImporter = UnityProjectImporter(project_path+'/Assets')
    # EditorApplication.OpenProject(project_path+'/Assets')
    
    # project_path = r'/Users/yushroom/program/Unity/FishEngine'
    # projectImporter = UnityProjectImporter(project_path+'')
    
    # importer = FishEditorInternal.AssetImporter.GetAtPath(scene_path)
    # importer.Import()
    

    # scene_path = os.path.join(project_path, 'scene', "01.unity")
    # scene_path = os.path.join(project_path, 'Assets/TestPhysics.unity')
    # scene_path = os.path.join(project_path, 'sponza.unity')
    # scene_path = os.path.join(project_path, 'PolygonSamurai', 'Scenes', 'Demo.unity')
    # scene_path = r'D:\program\FishEngine-Experiment\Script\FishEngine_demo1.unity'
    # sceneImporter = UnitySceneImporter(scene_path)
    # sceneImporter.Import()
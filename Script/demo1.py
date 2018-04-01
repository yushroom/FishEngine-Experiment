from FishEngine import *
import yaml
from collections import OrderedDict
# from FishEditor import AssetDataBase, SceneDumper
from Rotator2 import Rotator2
from FishEditor import EditorApplication

class Rotator3(Script):
    def __init__(self):
        super().__init__()
        self.speed = 1.0
        self.target = Vector3.zero()

    # def Update(self):
    #     self.transform.RotateAround(self.target, Vector3.up(), self.speed)
    #     self.transform.LookAt(self.target)

class Rotator3System(System):
    def Update(self):
        for go in GameObject.FindWithComponent(Rotator3):
            r = go.GetComponent(Rotator3)
            go.transform.RotateAround(r.target, Vector3.up(), r.speed)
            go.transform.LookAt(r.target)


class SceneLoader:
    def __init__(self):
        pass
    
    def Load(self, path):
        with open(path, 'r') as f:
            pass

def Start():
    project_path = '/Users/yushroom/program/Unity/FishEngine/Assets'
    # projectImporter = UnityProjectImporter(project_path+'/Assets')
    EditorApplication.OpenProject(project_path+'/Assets')

    cameraGO = GameObject(name="Camera")
    camera = cameraGO.AddComponent(Camera())
    cameraGO.transform.position = Vector3(0, 1, -10)
    cameraGO.transform.position = Vector3(0, 5, -5)
    cameraGO.transform.LookAt(Vector3.zero())
    # Object.Instantiate(cameraGO)

    lightGO = GameObject(name='Directional Light')
    lightGO.transform.localPosition = Vector3(0, 3, 0)
    lightGO.transform.localEulerAngles = Vector3(50, -30, 0)
    lightGO.AddComponent(Light())

    sun = GameObject.CreatePrimitive(PrimitiveType.Sphere)
    earth = GameObject.CreatePrimitive(PrimitiveType.Sphere)
    moon = GameObject.CreatePrimitive(PrimitiveType.Sphere)
    sun.name = 'Sun'
    earth.name = 'Earth'
    moon.name = 'Moon'

    earth.transform.position = Vector3(4, 0, 0)
    earth.transform.localScale = Vector3(0.5, 0.5, 0.5)
    moon.transform.localScale = Vector3(0.2, 0.2, 0.2)
    moon.transform.localPosition = Vector3(4.5, 0, 0)

    earth.transform.parent = sun.transform
    moon.transform.parent = earth.transform

    r:Rotator2 = earth.AddComponent(Rotator2())
    r.speed = 1
    moon.AddComponent(Rotator2()).speed = 2

    plane = GameObject.CreatePrimitive(PrimitiveType.Plane)

    # cameraGO.AddComponent(Rotator3())
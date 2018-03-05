import yaml

from FishEngine import *

from timing import timing

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


def PrintHierarchy():
    def PrintHierarchy_(transforms, indent):
        for t in transforms:
            print(' '*indent + t.name)
            PrintHierarchy_(t.children, indent+2)
    gos = SceneManager.GetActiveScene().GetRootGameObjects()
    PrintHierarchy_([go.transform for go in gos], 0)

def printT(t):
    print(t.name)
    print('  .localPosition:', t.localPosition)
    print('  .localEulerAngles:', t.localEulerAngles)
    print('  .localScale:', t.localScale)
    print('  .position:', t.position)
    print('  .eulerAngles:', t.eulerAngles)
    # print('  .localPosition:', t.localPosition)

def DefaultScene():
    camGO = GameObject('Main Camera')
    camGO.transform.localPosition = Vector3(0, 1, -10)
    camGO.AddComponent(Camera())
    lightGO = GameObject('Directional Light')
    lightGO.transform.localPosition = Vector3(0, 3, 0)
    lightGO.transform.localEulerAngles = Vector3(50, -30, 0)
    lightGO.AddComponent(Light())


from FishEditor import UnityProjectImporter, UnitySceneImporter, AssetDataBase

@timing
def Start():
    Screen.SetResolution(640//2, 1136//2, False)
    project_path = '/Users/yushroom/program/Unity/MonumentVally/Assets'
    projectImporter = UnityProjectImporter(project_path)

    # modelsPath = '/Users/yushroom/program/Unity/MonumentVally/Assets/Model/models.fbx'
    # modelsImporter = AssetDataBase.GUIDToImporter(AssetDataBase.AssetPathToGUID(modelsImporter))
    # importer.globalScale = 100

    scene_path = "/Users/yushroom/program/Unity/MonumentVally/Assets/Scene/02.unity"
    sceneImporter = UnitySceneImporter(scene_path)

    # gos = [t.gameObject for t in Object.FindObjectsOfType(RectTransform)]
    # for go in gos:
    #     print(go.components)

    # PrintHierarchy()
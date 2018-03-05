from FishEngine import *

def printT(t):
    print(t.name)
    print('  .localPosition:', t.localPosition)
    print('  .localEulerAngles:', t.localEulerAngles)
    print('  .localScale:', t.localScale)
    print('  .position:', t.position)
    print('  .eulerAngles:', t.eulerAngles)
    # print('  .localPosition:', t.localPosition)


class Rotator2(Script):
    def __init__(self):
        super().__init__()
        self.speed = 1.0

    def SystemUpdate(self):
        self.transform.RotateAround(self.transform.parent.position, Vector3.up(), self.speed)
        # print(self.name, self.transform.localPosition, self.transform.position)
        # printT(self.transform)

class Rotator2System(System):
    def Update(self):
        for go in GameObject.FindWithComponent(Rotator2):
            go.GetComponent(Rotator2).SystemUpdate()

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


def Start():
    cameraGO = GameObject("Camera")
    camera = cameraGO.AddComponent(Camera())
    cameraGO.transform.position = Vector3(0, 1, -10)
    print(Screen.width, Screen.height)
    cameraGO.transform.position = Vector3(0, 5, -5)
    cameraGO.transform.LookAt(Vector3.zero())
    # cameraGO.transform.eulerAngles = Vector3(45, 0, 0)
    print(camera.projectionMatrix)
    print(camera.worldToCameraMatrix)
    print(camera.transform.rotation)
    print(camera.transform.eulerAngles)

    lightGO = GameObject('Directional Light')
    lightGO.transform.localPosition = Vector3(0, 3, 0)
    lightGO.transform.localEulerAngles = Vector3(50, -30, 0)
    lightGO.AddComponent(Light())

    # print(Shader.__dict__)
    # m = Material.defaultMaterial()
    sun = GameObject.CreatePrimitive(PrimitiveType.Sphere)
    earth = GameObject.CreatePrimitive(PrimitiveType.Sphere)
    moon = GameObject.CreatePrimitive(PrimitiveType.Sphere)
    sun.name = 'Sun'
    earth.name = 'Earth'
    moon.name = 'Moon'
    # sun.AddComponent(LogMaterial())
    # earth = GameObject("Earth")
    earth.transform.position = Vector3(4, 0, 0)
    earth.transform.localScale = Vector3(0.5, 0.5, 0.5)
    # moon = GameObject("Moon")
    moon.transform.localScale = Vector3(0.2, 0.2, 0.2)
    moon.transform.localPosition = Vector3(4.5, 0, 0)

    earth.transform.parent = sun.transform
    moon.transform.parent = earth.transform
    assert(earth.transform.parent == sun.transform)
    assert(moon.transform.parent == earth.transform)

    printT(earth.transform)
    printT(moon.transform)

    earth.AddComponent(Rotator2()).speed = 1
    moon.AddComponent(Rotator2()).speed = 2

    plane = GameObject.CreatePrimitive(PrimitiveType.Plane)
    # cube = GameObject.CreatePrimitive(PrimitiveType.Cube)
    # print(len(Object.FindObjectsOfType(GameObject)))

    cameraGO.AddComponent(Rotator3())
    print(cameraGO)

    scene = SceneManager.GetActiveScene()
    scene.AddSystem(Rotator2System())
    # scene.AddSystem(Rotator3System())

    print(GameObject.ClassID)
    objs = FishEngineInternal.FindObjectsOfType(GameObject.ClassID)
    print(objs)

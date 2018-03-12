from FishEngine import *

class Rotator2(Script):
    # __slots__ = ('speed', '__hidden')
    def __init__(self):
        super().__init__()
        self.speed = 1.0
        self.__hidden = True    # test

    # def SystemUpdate(self):
    def Update(self):
        print(self.cpp.GetGameObject().name)
        self.transform.RotateAround(self.transform.parent.position, Vector3.up(), self.speed)
        # print(self.name, self.transform.localPosition, self.transform.position)
        # printT(self.transform)

# class Rotator2System(System):
#     def Update(self):
#         for go in GameObject.FindWithComponent(Rotator2):
#             go.GetComponent(Rotator2).SystemUpdate()

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
    cameraGO.transform.position = Vector3(0, 5, -5)
    cameraGO.transform.LookAt(Vector3.zero())

    assert(cameraGO.cpp.GetPyObject() is cameraGO)

    lightGO = GameObject('Directional Light')
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

    print(r.GetVisiableAttributes())

    plane = GameObject.CreatePrimitive(PrimitiveType.Plane)

    cameraGO.AddComponent(Rotator3())

    # scene = SceneManager.GetActiveScene()
    # scene.AddSystem(Rotator2System())

    objs = Object.FindObjectsOfType(GameObject)
    print(objs)

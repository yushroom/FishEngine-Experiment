import FishEngineInternal
from FishEngine import *
from Rotator import Rotator
from LogEulerAngles import LogEulerAngles

# import FishEngineInternal
import timeit
import sys, gc
# import objgraph
# import yaml
import weakref

from timing import timing
import demo1, demo2, demo3

from FishEditor import AssetDataBase


def Test():

    print(Vector3.back)

    v1 = Vector3(1, 2, 3)
    v2 = Vector3(1, 3, 5)
    print("v1.dot(v2) =", Vector3.Dot(v1, v2))

    count = 1000000
    start = timeit.default_timer()
    for _ in range(1, count):
        Vector3.Dot(v1, v2)
    elapsed = (timeit.default_timer() - start)
    print("elapsed1", elapsed)

    # test Object.FindObjectsOfType
    # test Object.DestroyImmediate
    objs = Object.FindObjectsOfType(GameObject)
    count = len(objs)
    print("objs:", objs)
    go2 = GameObject("go2")
    assert(go2 == go2.transform.gameObject)
    print("objs:", objs)
    assert(count+1 == len(objs))
    assert(sys.getrefcount(go2) == 3)   # go2, getrefcount, Scene
    t = go2.transform
    assert(sys.getrefcount(t) == 4)   # t, getrefcount, go2.__transform, go2.__components
    Object.DestroyImmediate(go2)
    del go2
    assert(count == len(objs))
    assert(t.gameObject is None)

def Start1():
    Test()
    go = GameObject("go")
    sys.getrefcount(go)
    rot = Rotator()
    rot.name = "Rotator"
    go.AddComponent(rot)
    assert(go == rot.gameObject)
    log = LogEulerAngles()
    log.name = "log"
    go.AddComponent(log)
    assert(len(go.components) == 3)


class LogMaterial(Script):
    def __init__(self):
        super().__init__()

    def Start(self):
        mr = self.gameObject.GetComponent(MeshRenderer)
        print(mr.material)
        mf = self.gameObject.GetComponent(MeshFilter)
        print(mf.mesh)

    def Update(self):
        pass


def Start():
    demo1.Start()
    # demo2.Start()
    # demo3.Start()

def Start4():
    cameraGO = GameObject("Camera")
    camera = cameraGO.AddComponent(Camera())
    # cameraGO.transform.position = Vector3(0, 0, 0)
    view = Matrix4x4.LookAt(Vector3.zero(), Vector3(0, 0, 1), Vector3.up())
    proj = Matrix4x4.Perspective(60, 1.5, 0.3, 1000)
    vp = proj * view
    print('view:', view)
    print('camera.view', camera.worldToCameraMatrix)
    print('proj', proj)
    print('camera.proj', camera.projectionMatrix)
    a = Vector3(0, 0, 0)
    for a in [Vector3(0, 0, 0.31), Vector3(0, 0, 1), Vector3(0, 0, 10), Vector3(0, 0, 999)]:
        # a = view.MultiplyPoint(a)
        print('view * a', view.MultiplyPoint(a))
        print('vp * a', vp.MultiplyPoint(a))
        print('')

    go = GameObject.CreatePrimitive(PrimitiveType.Sphere)
    go.transform.position = Vector3(-2, 0, -2)
    go = GameObject.CreatePrimitive(PrimitiveType.Sphere)
    go.transform.position = Vector3(0, 0 ,0)
    go = GameObject.CreatePrimitive(PrimitiveType.Sphere)
    go.transform.position = Vector3(2, 0 ,2)


# @timing
def Update():
    scene = SceneManager.GetActiveScene()
    for s in scene.systems:
        s.Update()
    
def Clean():
    # gos = Object.FindObjectsOfType(GameObject)
    # if len(gos) > 0:
    #     for go in gos:
    #         Object.DestroyImmediate(go)
    #     del gos
    #     del go
    SceneManager.GetActiveScene().Clean()
    SceneManager.StaticClean()
    Material.StaticClean()
    Mesh.StaticClean()
    AssetDataBase.StaticClean()
    
def Reload():
    print('Reload from Python.....')
    import importlib, sys, FishEngine
    importlib.reload(FishEngine)
    importlib.reload(demo2)
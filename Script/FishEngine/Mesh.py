import FishEngineInternal
import platform


# Mesh = FishEngineInternal.Mesh

# def __MeshFromTextFile(n):
#     with open('/Users/yushroom/program/FishEngine/assets/Models/{}.txt'.format(n), 'r') as f:
#         s = f.read()
#         mesh = Mesh.FromTextFile(s)
#     return mesh

# Mesh.sphere = __MeshFromTextFile('Sphere')
# Mesh.cube = __MeshFromTextFile('Cube')
# Mesh.capsule = __MeshFromTextFile('Capsule')
# Mesh.cylinder = __MeshFromTextFile('Cylinder')
# Mesh.plane = __MeshFromTextFile('Plane')
# Mesh.quad = __MeshFromTextFile('Quad')

from . import Object

class MeshManager:
    # __slots__ = ()

    # #internal use only
    # @staticmethod
    # def Wrap(mesh:FishEngineInternal.Mesh)->'Mesh':
    #     id = mesh.instanceID
    #     if id not in Mesh.wrappedMesh:
    #         m = Mesh()
    #         m.m_CachedPtr = mesh
    #         Mesh.wrappedMesh[id] = m
    #         return m
    #     return Mesh.wrappedMesh[id]

    @staticmethod
    def StaticClean():
        # del Mesh.wrappedMesh
        MeshManager.wrappedMesh = {}
        for n in ('Sphere', 'Cube', 'Capsule', 'Cylinder', 'Plane', 'Quad'):
            if hasattr(MeshManager, n):
                delattr(MeshManager, n)

    @staticmethod
    def GetInternalMesh(name:str):
        # print(Mesh.__dict__)
        if not hasattr(MeshManager, name):
            mesh = MeshManager.__MeshFromTextFile(name)
            mesh.name = name
            setattr(MeshManager, name, mesh)
        return getattr(MeshManager, name)

    @staticmethod
    def __MeshFromTextFile(n)->'Mesh':
        # print('__MeshFromTextFile', n)
        if platform.system() == 'Windows':
            q = r'D:\program\FishEngine-Experiment\Assets\Models\{}.txt'
        else:
            q = r'/Users/yushroom/program/FishEngine-Experiment/Assets/Models/{}.txt'
        with open(q.format(n), 'r') as f:
            m = FishEngineInternal.Mesh.FromTextFile(f.read())
        return m

def Mesh__new__(cls):
    return FishEngineInternal.CreateMesh()

def Mesh__init__(self):
    pass

Mesh = FishEngineInternal.Mesh
Mesh.__new__ = Mesh__new__
Mesh.__init__ = Mesh__init__
# Mesh.ClassID = FishEngineInternal.MeshClassID()
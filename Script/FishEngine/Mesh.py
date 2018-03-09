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

class Mesh(Object):
    __slots__ = ()
    wrappedMesh = {} # instanceID -> Mesh
    def __init__(self):
        super().__init__()

    #internal use only
    @staticmethod
    def Wrap(mesh:FishEngineInternal.Mesh)->'Mesh':
        id = mesh.instanceID
        if id not in Mesh.wrappedMesh:
            m = Mesh()
            m.m_CachedPtr = mesh
            Mesh.wrappedMesh[id] = m
            return m
        return Mesh.wrappedMesh[id]

    @staticmethod
    def StaticClean():
        # del Mesh.wrappedMesh
        Mesh.wrappedMesh = {}
        for n in ('Sphere', 'Cube', 'Capsule', 'Cylinder', 'Plane', 'Quad'):
            if hasattr(Mesh, n):
                delattr(Mesh, n)

    @staticmethod
    def GetInternalMesh(name:str):
        # print(Mesh.__dict__)
        if not hasattr(Mesh, name):
            mesh = Mesh.__MeshFromTextFile(name)
            mesh.name = name
            setattr(Mesh, name, mesh)
        return getattr(Mesh, name)

    @staticmethod
    def __MeshFromTextFile(n)->'Mesh':
        # print('__MeshFromTextFile', n)
        print(platform.system)
        if platform.system() == 'Windows':
            q = r'D:\program\FishEngine-Experiment\Assets\Models\{}.txt'
        else:
            q = r'/Users/yushroom/program/FishEngine-Experiment/Assets/Models/{}.txt'
        with open(q.format(n), 'r') as f:
            m = Mesh()
            mm = FishEngineInternal.Mesh.FromTextFile(f.read())
            m.m_CachedPtr = mm
            Mesh.wrappedMesh[mm.instanceID] = m
        return m
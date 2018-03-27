from . import Component, Mesh
import FishEngineInternal

# MeshFilter = FishEngineInternal.MeshFilter

class __MeshFilter:
    def Serialize(self, dumper):
        super(MeshFilter, self).Serialize(dumper)
        dumper.d('m_Mesh', self.mesh)

def MeshFilter__new__(cls):
    return FishEngineInternal.CreateMeshFilter()

def MeshFilter__init__(self):
    pass

MeshFilter = FishEngineInternal.MeshFilter
MeshFilter.__new__ = MeshFilter__new__
MeshFilter.__init__ = __MeshFilter.__init__
MeshFilter.ClassID = FishEngineInternal.MeshFilterClassID()
MeshFilter.Serialize = __MeshFilter.Serialize
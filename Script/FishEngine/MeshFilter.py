from . import Component, Mesh
import FishEngineInternal

# MeshFilter = FishEngineInternal.MeshFilter

class __MeshFilter:
    pass

def MeshFilter__new__(cls):
    return FishEngineInternal.CreateMeshFilter()

def MeshFilter__init__(self):
    pass

MeshFilter = FishEngineInternal.MeshFilter
MeshFilter.__new__ = MeshFilter__new__
MeshFilter.__init__ = __MeshFilter.__init__
MeshFilter.ClassID = FishEngineInternal.MeshFilterClassID()
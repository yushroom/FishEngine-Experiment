from . import Component, Material
import FishEngineInternal

class __MeshRenderer:
    pass


def MeshRenderer__new__(cls):
    return FishEngineInternal.CreateMeshRenderer()

def MeshRenderer__init__(self):
    pass

MeshRenderer = FishEngineInternal.MeshRenderer
MeshRenderer.__new__ = MeshRenderer__new__
MeshRenderer.__init__ = MeshRenderer__init__
MeshRenderer.ClassID = FishEngineInternal.MeshRendererClassID()
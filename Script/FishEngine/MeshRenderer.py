from . import Component, Material
import FishEngineInternal

class __MeshRenderer:
    def Serialize(self, dumper):
        super(MeshRenderer, self).Serialize(dumper)
        dumper.d('m_Materials', [])


def MeshRenderer__new__(cls):
    return FishEngineInternal.CreateMeshRenderer()

def MeshRenderer__init__(self):
    pass

MeshRenderer = FishEngineInternal.MeshRenderer
MeshRenderer.__new__ = MeshRenderer__new__
MeshRenderer.__init__ = MeshRenderer__init__
MeshRenderer.ClassID = FishEngineInternal.MeshRendererClassID()
MeshRenderer.Serialize = __MeshRenderer.Serialize
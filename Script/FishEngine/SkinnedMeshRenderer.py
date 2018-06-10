from . import Component, Material
import FishEngineInternal

class __SkinnedMeshRenderer:
    pass


def SkinnedMeshRenderer__new__(cls):
    return FishEngineInternal.CreateMeshRenderer()

def SkinnedMeshRenderer__init__(self):
    pass

class __SkinnedMeshRenderer:

    @property
    def bones(self):
        return FishEngineInternal.SkinnedMeshRenderer__GetBones(self);

SkinnedMeshRenderer = FishEngineInternal.SkinnedMeshRenderer
SkinnedMeshRenderer.__new__ = SkinnedMeshRenderer__new__
SkinnedMeshRenderer.__init__ = SkinnedMeshRenderer__init__
SkinnedMeshRenderer.ClassID = FishEngineInternal.SkinnedMeshRendererClassID()
SkinnedMeshRenderer.bones = __SkinnedMeshRenderer.bones

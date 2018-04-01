from . import Component
import FishEngineInternal

# FishEngineInternal.Camera.__slots__ = ()

class __Camera:
    @property
    def projectionMatrix(self):
        return self.m_CachedPtr.GetProjectionMatrix()

    @property
    def worldToCameraMatrix(self):
        return self.m_CachedPtr.GetWorldToCameraMatrix()

def Camera__new__(cls):
    return FishEngineInternal.CreateCamera()

def Camera__init__(self):
    pass

Camera = FishEngineInternal.Camera
Camera.__new__ = Camera__new__
Camera.__init__ = Camera__init__
Camera.ClassID = FishEngineInternal.CameraClassID()
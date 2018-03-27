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

    @staticmethod
    def SerializeAttr():
        return [
            ('near clip plane', 'nearClipPlane'),
            ('far clip plane', 'farClipPlane'),
            ('field of view', 'fieldOfView'),
            ('orthographic', 'orthographic'),
            ('orthographic size', 'orthographicSize'),
        ]

    def Serialize(self, dumper):
        super(Camera, self).Serialize(dumper)
        for name, attr in Camera.SerializeAttr():
            dumper.d(name, getattr(self, attr))

    def Deserialize(self, loader):
        # super().Deserialize(loader)
        for name, attr in Camera.SerializeAttr():
            setattr(self, attr, loader[name])

def Camera__new__(cls):
    return FishEngineInternal.CreateCamera()

def Camera__init__(self):
    pass

Camera = FishEngineInternal.Camera
Camera.__new__ = Camera__new__
Camera.__init__ = Camera__init__
Camera.ClassID = FishEngineInternal.CameraClassID()
Camera.SerializeAttr = __Camera.SerializeAttr
Camera.Serialize = __Camera.Serialize
Camera.Deserialize = __Camera.Deserialize
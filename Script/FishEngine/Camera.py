from . import Component
import FishEngineInternal

# FishEngineInternal.Camera.__slots__ = ()

class Camera(Component):
    __slots__ = ()
    ClassID = FishEngineInternal.CameraClassID()
    def __init__(self):
        """A Camera is a device through which the player views the world.
        """
        super().__init__()
        self.m_CachedPtr = FishEngineInternal.CreateCamera()
        self.m_CachedPtr.SetPyObject(self)

    @property
    def farClipPlane(self)->float:
        return self.cpp.GetFarClipPlane()
    @farClipPlane.setter
    def farClipPlane(self, value:float):
        self.cpp.SetFarClipPlane(value)

    @property
    def nearClipPlane(self)->float:
        return self.cpp.GetNearClipPlane()
    @nearClipPlane.setter
    def nearClipPlane(self, value:float):
        self.cpp.SetNearClipPlane(value)

    @property
    def fieldOfView(self)->float:
        return self.cpp.GetFieldOfView()
    @fieldOfView.setter
    def fieldOfView(self, value:float):
        self.cpp.SetFieldOfView(value)

    @property
    def orthographic(self)->bool:
        return self.cpp.GetOrthographic()
    @orthographic.setter
    def orthographic(self, value:bool):
        self.cpp.SetOrthographic(value)

    @property
    def orthographicSize(self)->float:
        return self.cpp.GetOrthographicSize()
    @orthographicSize.setter
    def orthographicSize(self, value:float):
        self.cpp.SetOrthographicSize(value)

    @property
    def projectionMatrix(self):
        return self.m_CachedPtr.GetProjectionMatrix()

    @property
    def worldToCameraMatrix(self):
        return self.m_CachedPtr.GetWorldToCameraMatrix()

    # @staticmethod
    # def splitCamelCase(s:str):
    #     ' '.join( re.sub('([a-z])([A-Z])', r'\1 \2', s).split() )

    # @staticmechod

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
        super().Serialize(dumper)
        for name, attr in Camera.SerializeAttr():
            dumper.d(name, getattr(self, attr))

    def Deserialize(self, loader):
        # super().Deserialize(loader)
        for name, attr in Camera.SerializeAttr():
            setattr(self, attr, loader[name])
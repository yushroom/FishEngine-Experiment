from . import Component
import FishEngineInternal

# FishEngineInternal.Camera.__slots__ = ()

class Camera(Component):
    __slots__ = ()
    ClassID = FishEngineInternal.Camera.ClassID
    def __init__(self):
        """A Camera is a device through which the player views the world.
        """
        super().__init__()
        self.m_CachedPtr = FishEngineInternal.CreateCamera()
        self.m_CachedPtr.SetPyObject(self)

    # fieldOfView = FishEngineInternal.Camera.fieldOfView
    @property
    def fieldOfView(self)->float:
        return self.m_CachedPtr.fieldOfView
    @fieldOfView.setter
    def fieldOfView(self, fov: float):
        self.m_CachedPtr.fieldOfView = fov

    @property
    def nearClipPlane(self)->float:
        return self.m_CachedPtr.nearClipPlane
    @nearClipPlane.setter
    def nearClipPlane(self, znear:float):
        self.m_CachedPtr.nearClipPlane = znear
    
    @property
    def farClipPlane(self)->float:
        return self.m_CachedPtr.farClipPlane
    @farClipPlane.setter
    def farClipPlane(self, zfar:float):
        self.m_CachedPtr.farClipPlane = zfar

    @property
    def orthographic(self)->bool:
        """Is the camera orthographic (true) or perspective (false)?
        """
        return self.m_CachedPtr.orthographic
    @orthographic.setter
    def orthographic(self, value:bool):
        self.m_CachedPtr.orthographic = value

    @property
    def orthographicSize(self)->float:
        return self.m_CachedPtr.orthographicSize
    @orthographicSize.setter
    def orthographicSize(self, value:float):
        # print('Camera.orthographicSize.setter')
        self.m_CachedPtr.orthographicSize = value

    @property
    def projectionMatrix(self):
        return self.m_CachedPtr.projectionMatrix()

    @property
    def worldToCameraMatrix(self):
        return self.m_CachedPtr.worldToCameraMatrix()

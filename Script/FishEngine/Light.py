from . import Component
import FishEngineInternal

class Light(Component):
    __slots__ = ()
    ClassID = FishEngineInternal.Light.ClassID
    def __init__(self):
        super().__init__()
        self.m_CachedPtr = FishEngineInternal.Light()
        self.m_CachedPtr.SetPyObject(self)
from . import Component

import FishEngineInternal

class Rigidbody(Component):
    def __init__(self):
        super().__init__()
        self.m_CachedPtr = FishEngineInternal.CreateRigidbody()
        self.m_CachedPtr.SetPyObject(self)
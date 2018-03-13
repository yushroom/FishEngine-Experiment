from . import Collider

import FishEngineInternal

class BoxCollider(Collider):
    def __init__(self):
        super().__init__()
        self.m_CachedPtr = FishEngineInternal.CreateBoxCollider()
        self.m_CachedPtr.SetPyObject(self)
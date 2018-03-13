from . import Collider, Vector3

import FishEngineInternal

class BoxCollider(Collider):
    ClassID = FishEngineInternal.BoxColliderClassID()
    def __init__(self):
        super().__init__()
        self.m_CachedPtr = FishEngineInternal.CreateBoxCollider()
        self.m_CachedPtr.SetPyObject(self)

    @property
    def center(self)->Vector3:
        return self.cpp.GetCenter()
    @center.setter
    def center(self, center:Vector3):
        self.cpp.SetCenter(center)

    @property
    def size(self)->Vector3:
        return self.cpp.GetSize()
    @size.setter
    def size(self, size:Vector3):
        self.cpp.SetSize(size)

    def Serialize(self, dumper):
        super().Serialize(dumper)
        dumper.d('m_Size', self.size)
        dumper.d('m_Center', self.center)

    def Deserialize(self, loader):
        self.size = loader['m_Size']
        self.center = loader['m_Center']

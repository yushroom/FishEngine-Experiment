from . import Collider, Vector3

import FishEngineInternal

class SphereCollider(Collider):
    ClassID = FishEngineInternal.SphereColliderClassID()
    def __init__(self):
        print('SphereCollider.__init__')
        super().__init__()
        self.m_CachedPtr = FishEngineInternal.CreateSphereCollider()
        self.m_CachedPtr.SetPyObject(self)

    @property
    def center(self)->Vector3:
        return self.cpp.GetCenter()
    @center.setter
    def center(self, center:Vector3):
        self.cpp.SetCenter(center)

    @property
    def radius(self)->float:
        return self.cpp.GetRadius()
    @radius.setter
    def radius(self, radius:float):
        self.cpp.SetRadius(radius)

    def Serialize(self, dumper):
        super().Serialize(dumper)
        dumper.d('m_Radius', self.radius)
        dumper.d('m_Center', self.center)

    def Deserialize(self, loader):
        self.radius = loader['m_Radius']
        self.center = loader['m_Center']

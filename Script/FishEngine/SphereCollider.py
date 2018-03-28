from . import Collider, Vector3

import FishEngineInternal

class __SphereCollider:
    def Serialize(self, dumper):
        super(SphereCollider, self).Serialize(dumper)
        dumper.d('m_Radius', self.radius)
        dumper.d('m_Center', self.center)

    def Deserialize(self, loader):
        self.radius = loader['m_Radius']
        self.center = loader['m_Center']

def SphereCollider__new__(cls):
    return FishEngineInternal.CreateSphereCollider()

def SphereCollider__init__(self):
    pass

SphereCollider = FishEngineInternal.SphereCollider
SphereCollider.__new__ = SphereCollider__new__
SphereCollider.__init__ = SphereCollider__init__
SphereCollider.ClassID = FishEngineInternal.SphereColliderClassID()
SphereCollider.Serialize = __SphereCollider.Serialize
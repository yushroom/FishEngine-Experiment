from . import Collider, Vector3

import FishEngineInternal

class __BoxCollider:
    def Serialize(self, dumper):
        dumper.d('m_Size', self.size)
        dumper.d('m_Center', self.center)

    def Deserialize(self, loader):
        self.size = loader['m_Size']
        self.center = loader['m_Center']

def BoxCollider__new__(cls):
    return FishEngineInternal.CreateBoxCollider()

def BoxCollider__init__(self):
    pass

BoxCollider = FishEngineInternal.BoxCollider
BoxCollider.__new__ = BoxCollider__new__
BoxCollider.__init__ = BoxCollider__init__
BoxCollider.ClassID = FishEngineInternal.BoxColliderClassID()
BoxCollider.Serialize = BoxCollider.Serialize
BoxCollider.Deserialize = BoxCollider.Deserialize
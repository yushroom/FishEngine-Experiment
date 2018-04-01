from . import Collider, Vector3

import FishEngineInternal

class __BoxCollider:
    pass

def BoxCollider__new__(cls):
    return FishEngineInternal.CreateBoxCollider()

def BoxCollider__init__(self):
    pass

BoxCollider = FishEngineInternal.BoxCollider
BoxCollider.__new__ = BoxCollider__new__
BoxCollider.__init__ = BoxCollider__init__
BoxCollider.ClassID = FishEngineInternal.BoxColliderClassID()
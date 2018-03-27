import FishEngineInternal

def Rigidbody__new__(cls):
    return FishEngineInternal.CreateRigidbody()

def Rigidbody__init__(self):
    pass

Rigidbody = FishEngineInternal.Rigidbody
Rigidbody.__new__ = Rigidbody__new__
Rigidbody.__init__ = Rigidbody__init__
Rigidbody.ClassID = FishEngineInternal.RigidbodyClassID()
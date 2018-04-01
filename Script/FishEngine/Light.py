from . import Component
import FishEngineInternal

class __Light:
    pass

def Light__new__(cls):
    return FishEngineInternal.CreateLight()

def Light__init__(self):
    pass

Light = FishEngineInternal.Light
Light.__new__ = Light__new__
Light.__init__ = Light__init__
Light.ClassID = FishEngineInternal.LightClassID()
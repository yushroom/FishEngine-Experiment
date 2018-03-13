from . import Component

import FishEngineInternal

class Rigidbody(Component):
    ClassID = FishEngineInternal.RigidbodyClassID()
    def __init__(self):
        super().__init__()
        self.m_CachedPtr = FishEngineInternal.CreateRigidbody()
        self.m_CachedPtr.SetPyObject(self)

    @property
    def mass(self)->float:
        return self.cpp.GetMass()
    @mass.setter
    def mass(self, value:float):
        self.cpp.SetMass(value)


    @property
    def drag(self)->float:
        return self.cpp.GetDrag()
    @drag.setter
    def drag(self, value:float):
        self.cpp.SetDrag(value)


    @property
    def angularDrag(self)->float:
        return self.cpp.GetAngularDrag()
    @angularDrag.setter
    def angularDrag(self, value:float):
        self.cpp.SetAngularDrag(value)


    @property
    def useGravity(self)->bool:
        return self.cpp.GetUseGravity()
    @useGravity.setter
    def useGravity(self, value:bool):
        self.cpp.SetUseGravity(value)


    @property
    def isKinematic(self)->bool:
        return self.cpp.GetIsKinematic()
    @isKinematic.setter
    def isKinematic(self, value:bool):
        self.cpp.SetIsKinematic(value)

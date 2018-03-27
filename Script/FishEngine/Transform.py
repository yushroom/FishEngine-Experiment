from . import Component, Vector3, Quaternion, Matrix4x4
# from . import FishEngineInternal
import FishEngineInternal
from enum import Enum, auto

# Transform = FishEngineInternal.Transform

# class RotationOrder(Enum):
#     OrderXYZ = auto
#     OrderXZY =
#     OrderYZX,
#     OrderYXZ,
#     OrderZXY,
#     OrderZYX,

class Space(Enum):
    World = auto()
    Self = auto()

class __Transform:
    # DO NOT USE IT directly !!!
    def __init__(self, cppObj: FishEngineInternal.Transform = None):
        super().__init__()
        if cppObj is not None:
            assert(cppObj.__class__ is FishEngineInternal.Transform)
            self.m_CachedPtr = cppObj
            cppObj.SetPyObject(self)
        self.m_RootOrder:int = 0

    # def __del__(self):
    #     print('Transform.__del__')

    # def Clean(self):
    #     # manually break ref cycle
    #     del self.__children

    @property
    def name(self):
        return self.gameObject.name

    @property
    def parent(self):
        p = self.GetParent()
    @parent.setter
    def parent(self, parent: 'Transform'):
        # parent: Transform or None
        self.SetParent(parent)

    def SetParent(self, parent: 'Transform', worldPositionStays:bool=True):
        t = None if parent is None else parent.m_CachedPtr
        self.m_CachedPtr.SetParent(t, worldPositionStays)

    @property
    def children(self):
        ret = []
        FishEngineInternal.Transform_GetChildren(self, ret)
        return ret

    @property
    def right(self)->Vector3:
        return self.rotation * Vector3.right()
    @right.setter
    def right(self, value:Vector3):
        self.rotation = Quaternion.FromToRotation(Vector3.right(), value)

    @property
    def up(self)->Vector3:
        return self.rotation * Vector3.up()
    @up.setter
    def up(self, value:Vector3):
        self.rotation = Quaternion.FromToRotation(Vector3.up(), value)

    @property
    def forward(self)->Vector3:
        return self.rotation * Vector3.right()
    @forward.setter
    def forward(self, value:Vector3):
        self.rotation = Quaternion.FromToRotation(value)

    @property
    def worldToLocalMatrix(self)->Matrix4x4:
        return self.m_CachedPtr.worldToLocalMatrix()

    @property
    def localToWorldMatrix(self)->Matrix4x4:
        return self.m_CachedPtr.localToWorldMatrix()

    # Returns the topmost transform in the hierarchy.
    @property
    def root(self)->'Transform':
        ret = self
        while ret.parent is not None:
            ret = ret.parent
        return ret

    @property
    def childCount(self)->int:
        pass

    @property
    def lossyScale(self)->Vector3:
        pass

    @property
    def hasChanged(self)->bool:
        pass

    # Moves the transform in the direction and distance of translation.
    def Translate(self, translation:Vector3, relativeTo:Space=Space.Self):
        if relativeTo == Space.World:
            self.position += translation
        else:
            self.position += self.TransformDirection(translation)

    # Moves the transform by x along the x axis, y along the y axis, and z along the z axis.
    # def Translate(self, x:float, y:float, z:float, relativeTo:Space=Space.Self):
    #     self.Translate(Vector3(x, y, z), relativeTo)

    def Rotate(self, eulerAngles:Vector3, relativeTo:Space=Space.Self):
        q = Quaternion.Euler(eulerAngles.x, eulerAngles.y, eulerAngles.z)
        if relativeTo == Space.Self:
            self.localRotation = self.localRotation * q
        else:
            r = self.rotation
            self.rotation = r * Quaternion.Inverse(r) * q * r


    # ?
    def LookAt(self, target: Vector3, worldUp: Vector3 = Vector3.up()):
        # m = Matrix4x4.LookAt(self.position, target, worldUp)
        # self.rotation = m.inverse.rotation()
        # forward = Vector3.Normalize(target - self.position)
        forward = target - self.position
        self.rotation = Quaternion.LookRotation(forward, worldUp)

    # Transforms direction from local space to world space.
    def TransformDirection(self, direction:Vector3)->Vector3:
        assert(False)

    # TransformDirection(x, y, z)

    # Transforms a direction from world space to local space. The opposite of Transform.TransformDirection.
    def InverseTransformDirection(self, direction:Vector3)->Vector3:
        assert(False)


    def RotateAround(self, point:Vector3, axis:Vector3, angle:float):
    # RotateAround = FishEngineInternal.Transform.RotateAround
        # print(point, axis, angle)
        self.m_CachedPtr.RotateAround(point, axis, angle)


    # @staticmethod
    # def Wrap(transform: FishEngineInternal.Transform):
    #     # transform: FishEngineInternal.Transform
    #     ret = Transform()
    #     ret.m_CachedPtr = transform
    #     return ret

    def SetSiblingIndex(self, index:int):
        # self.m_CachedPtr.m_RootOrder = index
        self.cpp.SetSiblingIndex(index)
    
    def GetSiblingIndex(self)->int:
        # return self.m_CachedPtr.m_RootOrder
        return self.cpp.GetSiblingIndex()

    def Serialize(self, dumper):
        super(Transform, self).Serialize(dumper)
        dumper.d('m_LocalRotation', self.localRotation)
        dumper.d('m_LocalPosition', self.localPosition)
        dumper.d('m_LocalScale', self.localScale)
        dumper.d('m_Children', self.children)
        dumper.d('m_Father', self.parent)
        dumper.d('m_RootOrder', self.GetSiblingIndex())


def Transform__new__(cls):
    return FishEngineInternal.CreateTransform()

def Transform__init__(self):
    pass

Transform = FishEngineInternal.Transform
Transform.__new__ = Transform__new__
Transform.__init__ = Transform__init__
Transform.ClassID = FishEngineInternal.TransformClassID()
# Transform.parent = __Transform.parent
Transform.children = __Transform.children
Transform.Serialize = __Transform.Serialize
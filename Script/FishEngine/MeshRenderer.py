from . import Component, Material
import FishEngineInternal

# MeshFilter = FishEngineInternal.MeshFilter

class MeshRenderer(Component):
    __slots__ = ('__material')
    ClassID = FishEngineInternal.MeshRenderer.ClassID
    def __init__(self):
        super().__init__()
        self.m_CachedPtr = FishEngineInternal.MeshRenderer()
        self.__material:Material = None
        self.m_CachedPtr.SetPyObject(self)

    @property
    def material(self)->Material:
        # return self.m_CachedPtr.material
        return self.__material
    @material.setter
    def material(self, value:Material):
        self.__material = value
        self.m_CachedPtr.material = value.m_CachedPtr
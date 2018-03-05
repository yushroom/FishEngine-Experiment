from . import Component, Mesh
import FishEngineInternal

# MeshFilter = FishEngineInternal.MeshFilter

class MeshFilter(Component):
    __slots__ = ('__mesh')
    ClassID = FishEngineInternal.MeshFilter.ClassID
    def __init__(self):
        super().__init__()
        self.__mesh:Mesh = None
        self.m_CachedPtr = FishEngineInternal.MeshFilter()
        self.m_CachedPtr.SetObject(self)

    @property
    def mesh(self):
        return self.__mesh
    @mesh.setter
    def mesh(self, value:Mesh):
        self.__mesh = value
        self.m_CachedPtr.mesh = value.m_CachedPtr
from . import Component, Mesh
import FishEngineInternal

# MeshFilter = FishEngineInternal.MeshFilter

class MeshFilter(Component):
    __slots__ = ('__mesh')
    ClassID = FishEngineInternal.MeshFilter.ClassID
    def __init__(self):
        super().__init__()
        self.__mesh:Mesh = None
        self.m_CachedPtr = FishEngineInternal.CreateMeshFilter()
        self.m_CachedPtr.SetPyObject(self)

    @property
    def mesh(self):
        return self.__mesh
    @mesh.setter
    def mesh(self, value:Mesh):
        self.__mesh = value
        self.cpp.SetMesh( value.cpp )

    def Serialize(self, dumper):
        super().Serialize(dumper)
        dumper.d('m_Mesh', self.__mesh)
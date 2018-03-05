import FishEngineInternal

from . import Mesh, Material, Camera, Matrix4x4, Light

class Graphics:
    # DrawMesh = FishEngineInternal.Graphics.DrawMesh
    @staticmethod
    def DrawMesh(mesh:Mesh, material:Material, submeshIndex:int, camera:Camera, mat:Matrix4x4, light:Light):
        FishEngineInternal.Graphics.DrawMesh(mesh.m_CachedPtr, material.m_CachedPtr, submeshIndex, camera.m_CachedPtr, mat, light.m_CachedPtr)
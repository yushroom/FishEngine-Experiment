#pragma once

namespace FishEngine
{
	class Mesh;
	class Material;
	class Matrix4x4;
	class Camera;
	class Light;
	
	class Graphics
	{
	public:
		static void DrawMesh(Mesh* mesh, Material* material, int subMeshIndex = -1);
		static void DrawMesh(Mesh* mesh, Material* material, int subMeshIndex, Camera* camera, const Matrix4x4& mat, Light* light);

	private:
		Graphics() = delete;
	};
}

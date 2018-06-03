#pragma once

#include <vector>
#include "Renderer.hpp"
#include "../Math/Matrix4x4.hpp"

namespace FishEditor
{
	class FBXImporter;
}

namespace FishEngine
{
	class Mesh;
	class Avatar;
	class RenderSystem;

	class SkinnedMeshRenderer : public Renderer
	{
	public:
		DeclareObject(SkinnedMeshRenderer, 137);

		SkinnedMeshRenderer() : Renderer(ClassID, ClassName)
		{
		}
		
		~SkinnedMeshRenderer() = default;

		void SetAvatar(Avatar* avatar) { m_Avatar = avatar; }
		Avatar* GetAvater() const { return m_Avatar; }

		void SetRootBone(Transform* rootBone) { m_RootBone = rootBone; }
		Transform* GetRootBone() const { return  m_RootBone; }

		void SetSharedMesh(Mesh* sharedMesh) { m_SharedMesh = sharedMesh; }
		Mesh* GetSharedMesh() const { return m_SharedMesh; }

	private:

		friend class RenderSystem;
		friend class FishEditor::FBXImporter;
		void UpdateMatrixPalette() const;

		// The mesh used for skinning.
		Mesh*		m_SharedMesh = nullptr;
		Avatar*		m_Avatar = nullptr;
		Transform*	m_RootBone = nullptr;

		// The bones used to skin the mesh.
		// same size with sharedMesh.bindposes
		std::vector<Transform*> m_Bones;

		mutable std::vector<Matrix4x4> m_MatrixPalette;
		mutable std::vector<Vector3> m_SkinnedVertexPosition;
		mutable std::vector<Vector3> m_SkinnedVertexNormal;
	};
}

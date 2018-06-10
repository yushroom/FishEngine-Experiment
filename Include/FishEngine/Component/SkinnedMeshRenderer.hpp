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
	class Animator;

	class SkinnedMeshRenderer : public Renderer
	{
	public:
		DeclareObject(SkinnedMeshRenderer, 137);

		SkinnedMeshRenderer() : Renderer(ClassID, ClassName)
		{
		}
		
		~SkinnedMeshRenderer() = default;

//		void SetAvatar(Avatar* avatar) { m_Avatar = avatar; }
//		Avatar* GetAvater() const { return m_Avatar; }

		void SetRootBone(Transform* rootBone) { m_RootBone = rootBone; }
		Transform* GetRootBone() const { return  m_RootBone; }

		void SetSharedMesh(Mesh* sharedMesh) { m_Mesh = sharedMesh; }
		Mesh* GetSharedMesh() const { return m_Mesh; }
		
		const std::vector<Transform*>& GetBones() const { return m_Bones; }

	private:
		
		void SetOverrideAvatar(Avatar* avatar);

		friend class RenderSystem;
		friend class FishEditor::FBXImporter;
		friend class Animator;
		void UpdateMatrixPalette() const;

		// The mesh used for skinning.
		Mesh*		m_Mesh = nullptr;
//		Avatar*		m_Avatar = nullptr;		// origin avatar
		Transform*	m_RootBone = nullptr;

		// The bones used to skin the mesh.
		// same size with sharedMesh.bindposes
		std::vector<Transform*> m_Bones;
		
//		bool 		m_UseBindPosesInAvatar = false;
		Avatar*		m_OverrideAvatar = nullptr;		// avatar in animation clips
		mutable std::vector<Matrix4x4> m_BindPoses;

		mutable std::vector<Matrix4x4> m_MatrixPalette;
		mutable std::vector<Vector3> m_SkinnedVertexPosition;
		mutable std::vector<Vector3> m_SkinnedVertexNormal;
	};
}

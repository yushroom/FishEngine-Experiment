#include <FishEngine/Component/SkinnedMeshRenderer.hpp>
#include <FishEngine/Render/Mesh.hpp>
#include <FishEngine/GameObject.hpp>


void FishEngine::SkinnedMeshRenderer::UpdateMatrixPalette() const
{
	if (m_MatrixPalette.size() != m_SharedMesh->GetBoneCount())
		m_MatrixPalette.resize(m_SharedMesh->GetBoneCount());
	
	const auto& worldToLocal = GetGameObject()->GetTransform()->GetWorldToLocalMatrix();
	const auto& bindposes = m_SharedMesh->m_bindposes;
	for (uint32_t i = 0; i < m_MatrixPalette.size(); ++i)
	{
		auto bone = m_Bones[i];
		auto& mat = m_MatrixPalette[i];
		// we multiply worldToLocal because we assume that the mesh is in local space in shader.
		mat = worldToLocal * bone->GetLocalToWorldMatrix() * bindposes[i];

#if Enable_GPU_Skinning
		// macOS bug
		// see the definition of Bones in ShaderVariables.inc
		mat = mat.transpose();
#endif
	}

#if ! Enable_GPU_Skinning

	auto mesh = m_SharedMesh;
	if (mesh->m_skinned)
	{
		if (m_SkinnedVertices.size() != mesh->m_vertexCount)
		{
			m_SkinnedVertices.resize(mesh->m_vertexCount);
		}

		for (int i = 0; i < mesh->m_vertexCount; i++)
		{
			auto boneIndex = mesh->m_boneWeights[i].boneIndex;
			auto boneWeight = mesh->m_boneWeights[i].weight;
			Matrix4x4 boneTransformation = m_MatrixPalette[boneIndex[0]] * boneWeight[0];
			boneTransformation += m_MatrixPalette[boneIndex[1]] * boneWeight[1];
			boneTransformation += m_MatrixPalette[boneIndex[2]] * boneWeight[2];
			boneTransformation += m_MatrixPalette[boneIndex[3]] * boneWeight[3];
			m_SkinnedVertices[i] = boneTransformation.MultiplyPoint3x4(mesh->m_vertices[i]);
		}
	}
	
	mesh->UpdateCPUSkinneing(m_SkinnedVertices);

#endif
}

#include <FishEngine/Component/SkinnedMeshRenderer.hpp>
#include <FishEngine/Render/Mesh.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Animation/Avatar.hpp>

using namespace FishEngine;

template<typename T, typename B>
B GetValueOrDefault(const std::map<T, B> & dict, const T & key, const B& defaultValue)
{
	auto it = dict.find(key);
	if (it != dict.end())
		return it->second;
	return defaultValue;
}

#if 1

Vector3 GetTranslation(const Matrix4x4& m)
{
	return Vector3(m[0][3], m[1][3], m[2][3]);
}

Matrix4x4 TranslationMatrix(const Vector3& t)
{
	Matrix4x4 mat;
	mat[0][3] = t[0];
	mat[1][3] = t[1];
	mat[2][3] = t[2];
	return mat;
}

Matrix4x4 GetTranslationMatrix(const Matrix4x4& m)
{
	Matrix4x4 mat;
	mat[0][3] = m[0][3];
	mat[1][3] = m[1][3];
	mat[2][3] = m[2][3];
	return mat;
}

Matrix4x4 GetRotationMatrix(const Matrix4x4& m)
{
	return Matrix4x4::FromRotation( m.ToRotation());
}

// ref: https://csyonghe.github.io/AnimationRetargeting/
void RetargetAnimation(Avatar* modelAvatar, Avatar* animationAvatar, Matrix4x4 outBindPoses[])
{
	constexpr int BoneCount = (int)HumanBodyBones::LastBone;
	const static int Parent[] = {-1, 0, 0, 1, 2, 3, 4, 0, 7, 8, 9, 10, 9, 9, 12, 13, 14, 15, 16, 17, 5, 6, 11, 11, 11, 18, 25, 26, 18, 28, 29, 18, 31, 32, 18, 34, 35, 18, 37, 38, 19, 40, 41, 19, 43, 44, 19, 46, 47, 19, 49, 50, 19, 52, 53};
	
	Matrix4x4 animAccumBindPose[BoneCount];	// l2w
	Matrix4x4 modelAccumBindPose[BoneCount];// l2w
	Matrix4x4 retargetedAccumBindPose[BoneCount];	// l2w
	
	for (int i = 0; i < BoneCount; ++i)
	{
		auto bone = animationAvatar->m_HumanSkeleton[i];
		if (bone != nullptr)
		{
			auto& l2w = animAccumBindPose[i];
			l2w = bone->GetLocalToWorldMatrix();
		}
		else
		{
			animAccumBindPose[i] = animAccumBindPose[Parent[i]];
		}
		
		bone = modelAvatar->m_HumanSkeleton[i];
		if (bone != nullptr)
		{
			auto& l2w = modelAccumBindPose[i];
			l2w = bone->GetLocalToWorldMatrix();
		}
		else
		{
			modelAccumBindPose[i] = modelAccumBindPose[Parent[i]];
		}
	}
	
//	retargetedBindPose[0] = GetTranslationMatrix(modelBindPose[0]) * GetRotationMatrix(animBindPose[0]);
//	retargetedAccumBindPose[0] = retargetedBindPose[0];
	retargetedAccumBindPose[0] = Matrix4x4::TRS(modelAvatar->m_HumanSkeletonBones[0].position, animationAvatar->m_HumanSkeletonBones[0].rotation, Vector3::one);
	
	for (int i = 1;  i < BoneCount; ++i)
	{
		int p = Parent[i];
//		Vector3 retargetedTranslation = (retargetedAccumBindPose[p].inverse() * modelAccumBindPose[p]).MultiplyPoint(GetTranslation(modelBindPose[i]));
//		retargetedBindPose[i] = TranslationMatrix(retargetedTranslation) * GetRotationMatrix(animBindPose[i]);
//		retargetedAccumBindPose[i] = retargetedAccumBindPose[p] * retargetedBindPose[i];
//		outBindPoses[i] = retargetedAccumBindPose[i].inverse();
		Vector3 retargetedTranslation = (retargetedAccumBindPose[p].inverse() * modelAccumBindPose[p]).MultiplyPoint(modelAvatar->m_HumanSkeletonBones[i].position);
		auto l2p = Matrix4x4::TRS(retargetedTranslation, animationAvatar->m_HumanSkeletonBones[i].rotation, Vector3::one);	// local to parent
		retargetedAccumBindPose[i] = retargetedAccumBindPose[p] * l2p;
	}
	
	for (int i = 0; i < BoneCount; ++i)
	{
		outBindPoses[i] = retargetedAccumBindPose[i].inverse();
	}
	
//	puts("here");
}

void FishEngine::SkinnedMeshRenderer::SetOverrideAvatar(Avatar* avatar)
{
	if (m_OverrideAvatar == avatar)
		return;
	
	m_OverrideAvatar = avatar;
//	m_BindPoses.resize(m_Mesh->m_bindposes.size());
	m_BindPoses = m_Mesh->m_bindposes;
	
	constexpr int BoneCount = (int)HumanBodyBones::LastBone;
	Matrix4x4 bindPoses[BoneCount];
	RetargetAnimation(m_Mesh->m_Avatar, avatar, bindPoses);
	
	for (int i = 0; i < m_Mesh->m_bindposes.size(); ++i)
	{
		Transform* bone = m_Bones[i];
		auto boneName = bone->GetName();
		auto boneId = GetValueOrDefault( m_Mesh->m_Avatar->m_BoneNameToHuman, boneName, HumanBodyBones::LastBone);
		if (boneId != HumanBodyBones::LastBone)
		{
			int id = static_cast<int>(boneId);
			m_BindPoses[i] = bindPoses[id];
//			auto& a = m_Mesh->m_bindposes[i];
//			auto& b = bindPoses[id];
//			puts("here");
		}
		else
		{
			puts(boneName.c_str());
		}
	}
}

#endif

void FishEngine::SkinnedMeshRenderer::UpdateMatrixPalette() const
{
	if (m_MatrixPalette.size() != m_Mesh->GetBoneCount())
		m_MatrixPalette.resize(m_Mesh->GetBoneCount());
	
	const auto& worldToObject = GetGameObject()->GetTransform()->GetWorldToLocalMatrix();
	const auto& bindposes = m_OverrideAvatar!=nullptr ? this->m_BindPoses : m_Mesh->m_bindposes;
//	const auto& bindposes = m_Mesh->m_bindposes;
	for (uint32_t i = 0; i < m_MatrixPalette.size(); ++i)
	{
		auto bone = m_Bones[i];
		auto& mat = m_MatrixPalette[i];
		// we multiply worldToObject because we assume that the mesh is in Object(local) space in shader.
		// bindpose: Object(Mesh) space -> bone local space
		// bone->GetLocalToWorldMatrix(): bone local space -> World(Game) space
		mat = worldToObject * bone->GetLocalToWorldMatrix() * bindposes[i];

#if Enable_GPU_Skinning
		// macOS bug
		// see the definition of Bones in ShaderVariables.inc
		mat = mat.transpose();
#endif
	}

#if ! Enable_GPU_Skinning

	auto mesh = m_Mesh;
	if (mesh->m_skinned)
	{
		if (m_SkinnedVertexPosition.size() != mesh->m_vertexCount)
		{
			m_SkinnedVertexPosition.resize(mesh->m_vertexCount);
		}
		
		if (m_SkinnedVertexNormal.size() != mesh->m_vertexCount)
		{
			m_SkinnedVertexNormal.resize(mesh->m_vertexCount);
		}

		for (int i = 0; i < mesh->m_vertexCount; i++)
		{
			auto boneIndex = mesh->m_boneWeights[i].boneIndex;
			auto boneWeight = mesh->m_boneWeights[i].weight;
			Matrix4x4 boneTransformation = m_MatrixPalette[boneIndex[0]] * boneWeight[0];
			boneTransformation += m_MatrixPalette[boneIndex[1]] * boneWeight[1];
			boneTransformation += m_MatrixPalette[boneIndex[2]] * boneWeight[2];
			boneTransformation += m_MatrixPalette[boneIndex[3]] * boneWeight[3];
			m_SkinnedVertexPosition[i] = boneTransformation.MultiplyPoint3x4(mesh->m_vertices[i]);
			m_SkinnedVertexNormal[i] = boneTransformation.MultiplyVector(mesh->m_normals[i]);
		}
	}
	
	mesh->UpdateCPUSkinneing(m_SkinnedVertexPosition, m_SkinnedVertexNormal);

#endif
}

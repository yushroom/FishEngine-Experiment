#pragma once

#include <unordered_map>
#include "FishEditor.hpp"
#include "ModelImporter.hpp"
//#include "FBXImporter/FBXImportData.hpp"
//#include <FishEngine/Path.hpp>
//#include <FishEngine/Prefab.hpp>
#include <FishEngine/Animation/AnimationCurve.hpp>
#include <string>
#include <map>
#include <FishEngine/Debug.hpp>
#include <FishEngine/Prefab.hpp>

namespace FishEngine
{
	class GameObject;
	class Transform;
	class Mesh;
	class Prefab;
	class Avatar;
	class AnimationClip;
	class SkinnedMeshRenderer;
}

namespace fbxsdk
{
	class FbxScene;
	class FbxNode;
	class FbxMesh;
	class FbxGeometry;
	class FbxAnimLayer;
	class FbxSurfaceMaterial;
}

namespace FishEditor
{
	struct FBXBoneAnimation
	{
		//FBXImportNode * node;
		FishEngine::Transform* node;
		FishEngine::TAnimationCurve<FishEngine::Vector3> translation;
		FishEngine::TAnimationCurve<FishEngine::Quaternion> rotation;
		//FishEngine::TAnimationCurve<FishEngine::Vector3> eulers;
		FishEngine::TAnimationCurve<FishEngine::Vector3> scale;
	};

	/** Animation clip containing a set of bone or blend shape animations. */
	struct FBXAnimationClip
	{
		std::string 	name;
		float 			start;
		float 			end;
		uint32_t 		sampleRate;
		std::vector<FBXBoneAnimation> boneAnimations;
		//std::vector<FBXBlendShapeAnimation> blendShapeAnimations;
	};

	struct ModelCollection
	{
		std::string 					name;
		FishEngine::Prefab* 			m_prefab = nullptr;
//		float globalScale;
//		bool useFileScale;
		FishEngine::GameObject* 		m_rootGameObject = nullptr;
//		FishEngine::Avatar*	m_avatar;
		std::unordered_map<fbxsdk::FbxNode*, FishEngine::Transform*>
										m_fbxNodeLookup;
		std::vector<FishEngine::Mesh*> 	m_meshes;
		std::unordered_map<fbxsdk::FbxMesh*, size_t>
										m_fbxMeshLookup; // fbxmesh -> index in m_meshes

		std::map<FishEngine::Mesh*, std::vector<uint32_t>>
										m_boneIndicesForEachMesh;
		std::vector<FishEngine::Transform*>
										m_bones;
		std::vector<FishEngine::Matrix4x4>
										m_bindposes;
		std::vector<FishEngine::SkinnedMeshRenderer*>
										m_skinnedMeshRenderers;

//		std::map<int, std::map<std::string, FishEngine::Object*>> m_objects;	// {classID: {name: Object}}
		std::map<std::string, FishEngine::GameObject*>
										m_gameObjects;
		std::vector<FBXAnimationClip> 	m_clips;
		std::vector<FishEngine::AnimationClip*>
										m_animationClips;
		FishEngine::Avatar*				m_avatar = nullptr;
	};


	class FBXImporter final : public ModelImporter
	{
	public:
		constexpr static int ClassID = 1041;
		constexpr static const char* ClassName = "FBXImporter";
		
		FBXImporter() : ModelImporter(ClassID, ClassName) { }
		~FBXImporter();

		
		virtual void Import() override;

		FishEngine::GameObject* GetRootGameObject() const
		{
			return m_model.m_rootGameObject;
		}
		
//		FishEngine::Mesh* GetMeshByName(const std::string& name)
//		{
//			auto it = m_meshes.find(name);
//			if (it == m_meshes.end())
//			{
//				return nullptr;
//			}
//			return it->second;
//		}

		FishEngine::Object* GetObjectByFileID(int fileID)
		{
			auto it = m_FileIDToObject.find(fileID);
			if (it == m_FileIDToObject.end())
			{
				LogError("fileID not found!");
				return nullptr;
			}
			return it->second;
		}

		void UpdateFileID(int fileID, std::string assetName)
		{
			if (assetName == "//RootNode")
				assetName = m_model.name;
			int classID = fileID / 100000;
			auto obj = m_Assets[classID][assetName];
			m_FileIDToObject[fileID] = obj;
			m_model.m_prefab->m_FileIDToObject = m_FileIDToObject;
		}

		
	protected:

		void BakeTransforms(fbxsdk::FbxScene* scene);

		FishEngine::Mesh* ParseMesh(fbxsdk::FbxMesh* fbxMesh);

		FishEngine::GameObject* ParseNode(fbxsdk::FbxNode* pNode);

		void GetLinkData(fbxsdk::FbxMesh* pGeometry, FishEngine::Mesh* mesh, std::map<uint32_t, uint32_t> const & vertexIndexRemapping);

		void ImportSkeleton(fbxsdk::FbxScene* scene);

		void UpdateBones(FishEngine::Transform* node);

		void ImportAnimations(fbxsdk::FbxScene* scene);
		void ImportAnimationLayer(fbxsdk::FbxAnimLayer* layer, fbxsdk::FbxNode* node, FBXAnimationClip& clip);
		FishEngine::AnimationClip* ConvertAnimationClip(const FBXAnimationClip& fbxClip);
		//void ImportAnimations(fbxsdk::FbxAnimLayer* layer, fbxsdk::FbxNode* node);

//		std::vector<std::pair<int, FishEngine::Object*>> m_fileIDToRecycleName;

		// {ClassID: {name: assetObject}}
		std::map<int, std::map<std::string, FishEngine::Object*>> m_Assets;

		int m_boneCount = 0;
		ModelCollection m_model;
//		std::map<std::string, FishEngine::Mesh*> m_meshes;
	};
}

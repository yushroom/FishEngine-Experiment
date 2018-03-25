#pragma once

#include <unordered_map>
#include "FishEditor.hpp"
#include "ModelImporter.hpp"
//#include "FBXImporter/FBXImportData.hpp"
//#include <FishEngine/Path.hpp>
//#include <FishEngine/Prefab.hpp>
//#include <FishEngine/Animation/AnimationCurve.hpp>
#include <string>
#include <map>
#include <FishEngine/Debug.hpp>

namespace FishEngine
{
	class Mesh;
	class Prefab;
	class GameObject;
	class Avatar;
	class Transform;
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
	struct ModelCollection
	{
		FishEngine::Prefab* m_prefab;
//		float globalScale;
//		bool useFileScale;
		FishEngine::GameObject* m_rootGameObject;
//		FishEngine::Avatar*	m_avatar;
		std::unordered_map<fbxsdk::FbxNode*, FishEngine::Transform*> m_fbxNodeLookup;
		std::vector<FishEngine::Mesh*> m_meshes;
		std::unordered_map<fbxsdk::FbxMesh*, size_t> m_fbxMeshLookup; // fbxmesh -> index in m_meshes
//		std::map<int, std::map<std::string, FishEngine::Object*>> m_objects;	// {classID: {name: Object}}
		std::map<std::string, FishEngine::GameObject*> m_gameObjects;
	};
	
	class FBXImporter : public ModelImporter
	{
	public:
		
		~FBXImporter();
		
		// for python
		static FBXImporter* Create()
		{
			return new FBXImporter();
		}
		
		void Import(const std::string& path);

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
			auto it = m_fileIDToObject.find(fileID);
			if (it == m_fileIDToObject.end())
			{
				LogError("fileID not found!");
				return nullptr;
			}
			return it->second;
		}

		const std::map<int, FishEngine::Object*>& GetFileIDToObject() const
		{
			return m_fileIDToObject;
		}
		
	protected:
		FishEngine::Mesh* ParseMesh(fbxsdk::FbxMesh* fbxMesh);
		FishEngine::GameObject* ParseNode(fbxsdk::FbxNode* pNode);
		void BakeTransforms(fbxsdk::FbxScene * scene);

//		std::vector<std::pair<int, FishEngine::Object*>> m_fileIDToRecycleName;

		std::map<int, FishEngine::Object*> m_fileIDToObject;
		ModelCollection m_model;
//		std::map<std::string, FishEngine::Mesh*> m_meshes;
	};
}

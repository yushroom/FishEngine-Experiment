#pragma once

//#include <unordered_map>
#include "FishEditor.hpp"
#include "ModelImporter.hpp"
//#include "FBXImporter/FBXImportData.hpp"
//#include <FishEngine/Path.hpp>
//#include <FishEngine/Prefab.hpp>
//#include <FishEngine/Animation/AnimationCurve.hpp>
#include <string>
#include <map>

namespace FishEngine
{
	class Mesh;
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
	class FBXImporter : public ModelImporter
	{
	public:
		
		~FBXImporter()
		{
			for(auto p : m_meshes)
			{
				delete p.second;
			}
		}
		
		// for python
		static FBXImporter* Create()
		{
			return new FBXImporter();
		}
		
		void Import(const std::string& path);
		
		FishEngine::Mesh* GetMeshByName(const std::string& name)
		{
			auto it = m_meshes.find(name);
			if (it == m_meshes.end())
			{
				return nullptr;
			}
			return it->second;
		}
		float m_globalScale = 1.0f;
		
	protected:
		FishEngine::Mesh* ParseMesh(fbxsdk::FbxMesh* fbxMesh);
		void ParseNode(fbxsdk::FbxNode* pNode);
		void BakeTransforms(fbxsdk::FbxScene * scene);
		
		float m_fileScale = 1.0f;
		
		
		std::map<std::string, FishEngine::Mesh*> m_meshes;
	};
}

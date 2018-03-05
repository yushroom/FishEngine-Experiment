#include <FishEditor/ModelImporter.hpp>

//#include <set>
//
//#include <boost/algorithm/string.hpp>
//#include <boost/lexical_cast.hpp>
//
//#include <FishEngine/Debug.hpp>
//#include <FishEngine/GameObject.hpp>
//#include <FishEngine/Common.hpp>
//#include <FishEngine/MeshRenderer.hpp>
//#include <FishEngine/SkinnedMeshRenderer.hpp>

//#include <boost/uuid/uuid_io.hpp>
#include <iostream>

using namespace FishEngine;

namespace FishEditor
{
//	void ModelImporter::Init()
//	{
//		auto importer = MakeShared<ModelImporter>();
//		auto nameAndIDs = std::vector<std::pair<PrimitiveType, int>>{
//			{PrimitiveType::Cube,     10202},
//			{PrimitiveType::Cylinder, 10206},
//			{PrimitiveType::Sphere,   10207},
//			{PrimitiveType::Capsule,  10208},
//			{PrimitiveType::Plane,    10209},
//			{PrimitiveType::Quad,     10210},
//			{PrimitiveType::Cone,     10211},
//			{PrimitiveType::ScreenAlignedQuad, 10212}
//		};
//		
//		for (auto & item : nameAndIDs)
//		{
//			int fileID = item.second;
//			auto & mesh = FishEngine::Mesh::s_builtinMeshes[item.first];
//			std::string guid = "0000000000000000e000000000000000";
//			std::istringstream sin(guid);
//			//using namespace boost::uuids;
//			sin >> importer->m_guid;
//			//std::cout << importer->m_guid;
//			auto meshName = mesh->name();
//			importer->m_fileIDToRecycleName[fileID] = meshName;
//			importer->m_recycleNameToFileID[meshName] = fileID;
//			auto path = "InternalModel";
//			AssetImporter::s_objectInstanceIDToPath[mesh->GetInstanceID()] = path;
//			AssetImporter::s_pathToImpoter[path] = importer;
//		}
//		importer->m_nextMeshFileID = 10213;
//	}

	
//	ModelImporter& ModelImporter::operator=(ModelImporter const & rhs)
//	{
//		m_globalScale = rhs.m_globalScale;
//		m_fileScale = rhs.m_fileScale;
//		m_importNormals = rhs.m_importNormals;
//		m_importTangents = rhs.m_importTangents;
//		m_materialSearch = rhs.m_materialSearch;
//		return *this;
//	}
}

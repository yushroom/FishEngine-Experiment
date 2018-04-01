#include <FishEditor/AssetDatabase.hpp>

#include <FishEditor/AssetImporter.hpp>

#include <regex>
#include <FishEngine/Util/StringFormat.hpp>
#include <FishEngine/Render/Mesh.hpp>
#include <FishEditor/ModelImporter.hpp>
#include <FishEditor/Serialization/NativeFormatImporter.hpp>

#include <FishEngine/Prefab.hpp>
#include <FishEngine/Debug.hpp>

using namespace FishEngine;

namespace FishEditor
{
    FileNode* AssetDatabase::s_AssetRootDir = nullptr;
	std::unordered_map<std::string, std::string> AssetDatabase::s_PathToGUID;
	std::unordered_map<std::string, std::string> AssetDatabase::s_GUIDToPath;
	std::unordered_map<std::string, AssetImporter*> AssetDatabase::s_GUIDToImporter;
	std::unordered_map<int, AssetImporter*> AssetDatabase::s_AssetInstanceIDToImporter;

	FishEngine::Object* AssetDatabase::LoadMainAssetAtPath(const std::string& path)
	{
		auto importer = AssetImporter::GetAtPath(path);
		assert(importer != nullptr);
		return importer->GetMainAsset();
	}
	
	std::string AssetDatabase::AssetPathToGUID(const std::string& path)
	{
		auto it = s_PathToGUID.find(path);
		if (it == s_PathToGUID.end())
		{
			abort();
			return "";
		}
		return it->second;
	}

	std::string AssetDatabase::GUIDToAssetPath(const std::string& guid)
	{
		auto it = s_GUIDToPath.find(guid);
		if (it == s_GUIDToPath.end())
		{
			abort();
			return "";
		}
		return it->second;
	}

	std::string AssetDatabase::GetAssetPathFromInstanceID(int instanceID)
	{
		//s_AssetInstanceIDToImporter.find(instanceID);
		return s_AssetInstanceIDToImporter[instanceID]->GetAssetPath();
	}

	void AssetDatabase::StaticInit()
	{
		{
#ifdef _WIN32
			q = R'(D:\program\FishEngine-Experiment\Assets\Models\{}.txt)'
#else
			const char *modelsRootDir = "/Users/yushroom/program/FishEngine-Experiment/Assets/Models/{}.txt";
#endif
			const char *guid = "0000000000000000e000000000000000";
			std::map<int, const char *> id2name = {
					{10202, "Cube"},
					{10206, "Cylinder"},
					{10207, "Sphere"},
					{10208, "Capsule"},
					{10209, "Plane"},
					{10210, "Quad"},
			};
			auto importer = new ModelImporter();
			importer->SetGUID(guid);
			auto prefab = new Prefab();
			importer->m_MainAsset = prefab;

			for (auto &&p : id2name)
			{
				auto name = p.second;
				int fileID = p.first;
				auto path = FishEngine::Format(modelsRootDir, name);
				auto str = ReadFileAsString(path);
				FishEngine::Mesh *mesh = FishEngine::Mesh::FromTextFile(str);
				importer->m_FileIDToObject[fileID] = mesh;
				prefab->AddObject(fileID, mesh);
			}

			AssetImporter::AddImporter(importer, guid);
		}

		{
			const char *guid = "0000000000000000f000000000000000";
			auto importer = new NativeFormatImporter();
			importer->SetGUID(guid);
			auto mat = FishEngine::Material::GetDefaultMaterial();
			importer->m_MainAsset = mat;
			importer->m_FileIDToObject[10303] = mat;
			AssetImporter::AddImporter(importer, guid);
		}
	}

	void AssetDatabase::StaticClean()
	{
		for (auto&& p : s_GUIDToImporter)
		{
			delete p.second;
		}
		s_GUIDToImporter.clear();
		s_PathToGUID.clear();
		s_GUIDToPath.clear();
		s_AssetInstanceIDToImporter.clear();
	}

	inline std::string ReplaceSep(std::string path)
	{
		fs::path p(path);
		assert(!p.is_absolute());
		p.remove_trailing_separator();
		auto pp = p.string();
		std::regex sep_pattern(R"(\\)");
		std::regex_replace(path.begin(), pp.begin(), pp.end(), sep_pattern, "/");
		return path;
	}

	void AssetDatabase::AddAssetPathAndGUIDPair(const std::string& path, const std::string& guid)
	{
		auto p = ReplaceSep(path);
		s_PathToGUID[p] = guid;
		s_GUIDToPath[guid] = p;
	}

	FishEngine::Object* AssetDatabase::GetAssetByGUIDAndFileID(const std::string& guid, int64_t fileID)
	{
		if (guid == "0000000000000000f000000000000000")
		{
			LogWarning("0000000000000000f000000000000000");
			assert(fileID == 10303);
		}
		auto importer = AssetImporter::GetByGUID(guid);	// maybe internal assset
		if (importer == nullptr)
		{
			auto path = GUIDToAssetPath(guid);
//			auto obj = LoadMainAssetAtPath(path);
			importer = AssetImporter::GetAtPath(path);
		}
		assert(importer != nullptr);
		auto obj = importer->GetObjectByFileID(fileID);
		return obj;
//		auto prefab = importer->GetMainAsset()->As<Prefab>();
//		assert(prefab != nullptr);
//		return prefab->GetObjectByFileID(fileID);
	}
}

#include <FishEditor/AssetDatabase.hpp>

#include <FishEditor/AssetImporter.hpp>

#include <regex>

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
}

#include <FishEditor/AssetDatabase.hpp>

namespace FishEditor
{
    FileNode* AssetDatabase::s_AssetRootDir = nullptr;
	std::unordered_map<std::string, std::string> AssetDatabase::s_PathToGUID;
	std::unordered_map<std::string, std::string> AssetDatabase::s_GUIDToPath;

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
}
#include <FishEditor/AssetImporter.hpp>

#include <FishEditor/FileNode.hpp>
#include <FishEditor/AssetDatabase.hpp>
#include <FishEditor/FBXImporter.hpp>
#include <yaml-cpp/yaml.h>
#include <fstream>

#include <boost/algorithm/string.hpp>

namespace FishEditor
{
	std::unordered_map<std::string, AssetImporter*> AssetImporter::s_GUIDToImporter;

	AssetImporter* AssetImporter::GetAtPath(const std::string& path)
	{
		auto p = fs::path(path);
		auto ext = boost::to_lower_copy(p.extension().string());
		auto meta_file = fs::path(path + ".meta");
		if (fs::exists(meta_file))
		{
			//auto modified_time = fs::last_write_time(meta_file);
			std::fstream fin(meta_file.string());
			auto nodes = YAML::LoadAll(fin);
			auto&& node = nodes.front();
			auto timeCreated = node["timeCreated"].as<uint32_t>();
			auto guid = node["guid"].as<std::string>();

			AssetImporter* importer = nullptr;
			if (ext == ".fbx")
			{
				importer = new FBXImporter();
			}
			else
			{
				importer = new AssetImporter();
			}
			importer->m_GUID = guid;
			importer->m_AssetTimeStamp = timeCreated;
			importer->m_AssetPath = path;

			AssetDatabase::s_GUIDToPath[guid] = path;
			AssetDatabase::s_PathToGUID[path] = guid;
			AddImporter(importer, guid);
			return importer;
		}
		return nullptr;
	}

	AssetImporter* AssetImporter::GetByGUID(const std::string& guid)
	{
		auto it = s_GUIDToImporter.find(guid);
		if (it == s_GUIDToImporter.end())
		{
			auto path = AssetDatabase::GUIDToAssetPath(guid);
			auto importer = AssetImporter::GetAtPath(path);
			s_GUIDToImporter[guid] = importer;
			return importer;
		}
		return it->second;
	}

	void AssetImporter::AddImporter(AssetImporter* importer, const std::string& guid)
	{
		s_GUIDToImporter[guid] = importer;
	}
}
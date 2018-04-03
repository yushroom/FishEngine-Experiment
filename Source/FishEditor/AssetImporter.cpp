#include <FishEditor/AssetImporter.hpp>
#include <FishEditor/FileNode.hpp>
#include <FishEditor/AssetDatabase.hpp>
#include <FishEditor/FBXImporter.hpp>
#include <FishEditor/Path.hpp>
#include <FishEditor/Serialization/NativeFormatImporter.hpp>
#include <FishEditor/Serialization/DefaultImporter.hpp>

#include <yaml-cpp/yaml.h>
#include <fstream>

#include <boost/algorithm/string.hpp>

#include <FishEngine/Application.hpp>

namespace FishEditor
{
	std::unordered_map<std::string, AssetImporter*> AssetImporter::s_GUIDToImporter;


	std::string AssetImporter::CorrectAssetPath(const std::string& path)
	{
		fs::path p(path);
		if (p.is_absolute())
		{
			LogWarning("The path is absolute. All paths should be relative to the project folder, for example: \"Assets/MyTextures/hello.png\"");
			auto assetRoot = FishEngine::Application::GetInstance().GetDataPath();
			auto rel = fs::relative(path, assetRoot);
			return rel.string();
		}
		return path;
	}


	AssetImporter::~AssetImporter()
	{
//		for (auto&& p : m_FileIDToObject)
//		{
//			delete p.second;
//		}
	}

	void AssetImporter::SetAssetPath(const std::string& value)
	{
		m_AssetPath = CorrectAssetPath(value);
	}

	std::string AssetImporter::GetFullPath() const
	{
		fs::path p(FishEngine::Application::GetInstance().GetDataPath());
		p = p.parent_path();
		p.append(m_AssetPath);
		return p.string();
	}

	AssetImporter* AssetImporter::GetAtPath(std::string path)
	{
		path = AssetImporter::CorrectAssetPath(path);
		auto guid = AssetDatabase::AssetPathToGUID(path);
		auto it = s_GUIDToImporter.find(guid);
		if (it != s_GUIDToImporter.end())
			return it->second;

		auto root = FishEngine::Application::GetInstance().GetDataPath()+"/..";
		auto p = fs::path(root);
		p.append(path);
		auto ext = boost::to_lower_copy(p.extension().string());
		auto meta_file = fs::path(p.string() + ".meta");
		if (fs::exists(meta_file))
		{
			//auto modified_time = fs::last_write_time(meta_file);
			std::fstream fin(meta_file.string());
			auto nodes = YAML::LoadAll(fin);
			auto&& meta = nodes.front();
			auto timeCreated = meta["timeCreated"].as<uint32_t>();
			auto guid = meta["guid"].as<std::string>();

			AssetImporter* importer = nullptr;
			if (ext == ".fbx")
			{
				auto fbximporter = new FBXImporter();
				auto node = meta["ModelImporter"];
				auto meshes = node["meshes"];
				float globalScale = meshes["globalScale"].as<float>();
				bool useFileScale = meshes["useFileScale"].as<int>() == 1;
				fbximporter->SetGlobalScale(globalScale);
				fbximporter->SetUseFileScale(useFileScale);
				importer = fbximporter;
			}
			else if (ext == ".prefab")
			{
//				importer = new AssetImporter();
				auto nativeImporter = new NativeFormatImporter();
				importer = nativeImporter;
			}
			else if (ext == ".unity")
			{
				auto defaultImporter = new DefaultImporter();
				importer = defaultImporter;
			}
//			if (importer == nullptr)
//				return nullptr;
			assert(importer != nullptr);
			importer->m_GUID = guid;
			importer->m_AssetTimeStamp = timeCreated;
			importer->m_AssetPath = path;
			importer->Import();

			AssetDatabase::AddAssetPathAndGUIDPair(path, guid);
			AddImporter(importer, guid);
			return importer;
		}
		else
		{
			LogError(".meta file not found!");
			abort();
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
		importer->m_GUID = guid;
	}
}

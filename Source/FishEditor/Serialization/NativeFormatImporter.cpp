#include <FishEditor/Serialization/NativeFormatImporter.hpp>
#include <FishEditor/Serialization/YAMLArchive.hpp>
#include <FishEditor/Path.hpp>

using namespace FishEngine;

namespace FishEditor
{
	struct MetaData
	{
		int				fileFormatVersion;
		std::string		guid;
		uint64_t		timeCreated;
		std::string		importerName;
		YAML::Node		importerInfo;
	};

	// import ".meta" file
	MetaData ImportMeta(const std::string& path)
	{
		auto node = YAML::LoadAllFromFile(path)[0];
		MetaData meta;
		meta.fileFormatVersion = node["fileFormatVersion"].as<int>();
		meta.guid = node["guid"].as<std::string>();
		meta.timeCreated = node["timeCreated"].as<uint64_t>();
		meta.importerInfo = node;
		return meta;
	}

	FishEngine::Object* NativeFormatImporter::Import()
	{
		auto meta = ImportMeta(m_AssetPath + ".meta");

		YAMLInputArchive archive;

		auto objects = archive.LoadAll(m_AssetPath);
		auto mainObjectFileID = meta.importerInfo["NativeFormatImporter"]["mainObjectFileID"].as<int64_t>();
		auto mainObject = archive.GetObjectByFileID(mainObjectFileID);
		return mainObject;

		//auto ext = fs::path(m_AssetPath).extension();
		//if (ext == ".prefab")
		//{
		//	
		//}
		//return nullptr;
	}
}

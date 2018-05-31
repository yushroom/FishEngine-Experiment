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

	void NativeFormatImporter::Import()
	{
		auto path = GetFullPath();
		auto meta = ImportMeta(path + ".meta");

		YAMLInputArchive archive;

		auto fullpath = this->GetFullPath();
		auto str = ReadFileAsString(fullpath);
		auto objects = archive.LoadAllFromString(str);
		auto mainObjectFileID = meta.importerInfo["NativeFormatImporter"]["mainObjectFileID"].as<int64_t>();
		Object* mainObject = nullptr;
		if (mainObjectFileID == 0)		// fileFormatVersion == 2
			mainObject = objects[0];
		else
			mainObject = archive.GetObjectByFileID(mainObjectFileID);
		assert(mainObject != nullptr);
		m_MainAsset = mainObject;

		if (mainObject->GetClassID() == Prefab::ClassID)
		{
			Prefab* prefab = mainObject->As<Prefab>();
			//for (auto &&p : archive.GetFileIDToObject())
			//{
			//	prefab->AddObject(p.first, p.second);
			//}
			prefab->m_FileIDToObject = archive.GetFileIDToObject();
		}
	}
}

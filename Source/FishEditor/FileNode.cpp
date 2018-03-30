#include <FishEditor/FileNode.hpp>

#include <FishEditor/AssetDatabase.hpp>
#include <FishEngine/Application.hpp>

#include <yaml-cpp/yaml.h>
#include <fstream>
//#include <boost/filesystem.hpp>

#include <FishEngine/Debug.hpp>

FishEditor::FileNode::FileNode(const Path & rootDir) : path(rootDir)
{
	fileName = path.stem().string();

	std::string guid;
	auto meta_file = fs::path(path.string() + ".meta");
	if (fs::exists(meta_file))
	{
		//auto modified_time = fs::last_write_time(meta_file);
		std::fstream fin(meta_file.string());
		auto nodes = YAML::LoadAll(fin);
		auto&& node = nodes.front();
		auto meta_created_time = node["timeCreated"].as<uint32_t>();
		guid = node["guid"].as<std::string>();

		auto rel = fs::relative(path, FishEngine::Application::GetInstance().GetDataPath()+"/..");
		AssetDatabase::AddAssetPathAndGUIDPair(rel.string(), guid);
	}
	else
	{
		LogWarning(FishEngine::Format(".meta not found for file[{}]", path.string()));
	}


	if (!fs::is_directory(path))
	{
		return;
	}

	fs::directory_iterator end;
	for (fs::directory_iterator it(path); it != end; ++it)
	{
		auto p = it->path();
		auto fn = p.filename();
		if (fn.c_str()[0] == '.')		// hidden file
			continue;
		auto ext = fn.extension();
		if (ext == ".meta" || ext == ".DS_store")	// .meta file
			continue;

		auto n = new FileNode(p);
		n->parent = this;
		n->guid = guid;
		if (fs::is_directory(p))
		{
			n->isDir = true;
			subdirs.push_back(n);
		}
		else
		{
			n->isDir = false;
			files.push_back(n);
		}
	}
}

#include <FishEditor/Serialization/YAMLArchive.hpp>

#include <FishEditor/AssetDatabase.hpp>
#include <FishEditor/AssetImporter.hpp>
#if _WIN32
#	undef GetClassName
#endif

#include <FishEngine/CreateObject.hpp>


namespace FishEditor
{
	std::vector<std::pair<int, int64_t>> GetClassIDAndFileID(const std::string& s)
	{
		std::vector<std::pair<int, int64_t>> pairs;

		std::regex pattern(R"(--- !u!(\d+) &(\d+))");
		auto words_begin =
			std::sregex_iterator(s.begin(), s.end(), pattern);
		auto words_end = std::sregex_iterator();
		int count = std::distance(words_begin, words_end);
		pairs.reserve(count);
		for (std::sregex_iterator i = words_begin; i != words_end; ++i)
		{
			std::smatch match = *i;
			int classID = std::stoi(match[1]);
			long long fileID = std::stoll(match[2]);
			pairs.emplace_back(classID, fileID);
		}
		return pairs;
	}


	void PatchNode(YAML::Node& node)
	{
		auto className = node.begin()->first.as<std::string>();
		if (className == "GameObject")
		{
//			m_Component:
//			- component: {fileID: 822216906}
//			- 13: {fileID: 822216907}
			auto n = node["GameObject"]["m_Component"];
			for (auto it = n.begin(); it != n.end(); ++it)
			{
				auto item = *it;
				if (!item["fileID"])
				{
					item["fileID"] = item.begin()->second["fileID"];
//					int fileID = item["fileID"].as<int>();
//					printf("%d\n", fileID);
				}
			}
		}
		else if (className == "Camera")
		{
			auto n = node["Camera"];
			if (n["near clip plane"] && !n["m_NearClipPlane"])
				n["m_NearClipPlane"] = n["near clip plane"];
			if (n["far clip plane"] && !n["m_FarClipPlane"])
				n["m_FarClipPlane"] = n["far clip plane"];
			if (n["field of view"] && !n["m_FieldOfView"])
				n["m_FieldOfView"] = n["field of view"];
			if (n["orthographic"] && !n["m_Orthographic"])
				n["m_Orthographic"] = n["orthographic"];
			if (n["orthographic size"] && !n["m_OrthographicSize"])
				n["m_OrthographicSize"] = n["orthographic size"];
		}
	}

	std::vector<Object*> YAMLInputArchive::LoadAll(const std::string& path)
	{
		std::ifstream is(path);
		assert(is.good());
		std::stringstream buffer;
		buffer << is.rdbuf();
		auto classID_fileID = GetClassIDAndFileID(buffer.str());
		m_nodes = YAML::LoadAll(buffer.str());
		assert(m_nodes.size() == classID_fileID.size());

		for (auto&& node : m_nodes)
			PatchNode(node);

		std::vector<Object*> objects(m_nodes.size());

		for (int i = 0; i < m_nodes.size(); ++i)
		{
			int classID = classID_fileID[i].first;
			int64_t fileID = classID_fileID[i].second;
			
			Object* obj = nullptr;
			
			if (classID == Prefab::ClassID)
			{
				auto&& node = m_nodes[i];
				auto&& parentPrefab = node.begin()->second["m_ParentPrefab"];
				auto parentFileID = parentPrefab["fileID"].as<int64_t>();
				if (parentFileID == 0)
				{
					obj = CreateEmptyObject<Prefab>();
				}
				else
				{
					std::string guid = parentPrefab["guid"].as<std::string>();
					std::string assetPath = AssetDatabase::GUIDToAssetPath(guid);
					//LogError("[TODO] make a copy of main asset");
					Prefab* prefab = dynamic_cast<Prefab*>( AssetDatabase::LoadMainAssetAtPath(assetPath));
					Prefab* instance  = prefab->Instantiate();
					obj = instance;
				}
			}
			else
			{
				obj = CreateEmptyObjectByClassID(classID);
			}

			objects[i] = obj;
			m_FileIDToObject[fileID] = obj;
		}

		for (int i = 0; i < m_nodes.size(); ++i)
		{
			auto&& node = m_nodes[i];
			assert(node.IsMap());
			auto className = node.begin()->first.as<std::string>();
			Object* obj = objects[i];

			if (obj != nullptr)
			{
				assert(className == obj->GetClassName());
				PushNode(node.begin()->second);
				obj->Deserialize(*this);
				PopNode();
			}
		}
		return objects;
	}




	void YAMLOutputArchive::Dump(Object* obj)
	{
		todo.push_back(obj);

		while (!todo.empty())
		{
			auto o = todo.back();
			todo.pop_back();

			auto it = done.find(o);
			if (it == done.end())
			{
				done.insert(o);
				NewLine();
				fout << Format("--- !u!{} &{}\n", o->GetClassID(), o->GetInstanceID());
				beginOfLine = true;
				this->BeforeKey();
				Serialize(o->GetClassName());
				this->AfterKey();
				o->Serialize(*this);
				this->AfterValue();
			}
		}
	}

}

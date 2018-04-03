#include <FishEditor/Serialization/YAMLArchive.hpp>

#include <FishEditor/AssetDatabase.hpp>
#include <FishEditor/AssetImporter.hpp>
#if _WIN32
#	undef GetClassName
#endif

#include <FishEngine/CreateObject.hpp>

#include <FishEngine/Serialization/Serialize.hpp>

#include <boost/algorithm/string.hpp>



bool YAMLNodeHasKey(const YAML::Node& node, const char* key)
{
	assert(node.IsMap());
	for (auto it = node.begin(); it != node.end(); ++it)
	{
		if (it->first.as<std::string>() == key)
		{
			return true;
		}
	}
	return false;
}


namespace FishEditor
{
	std::vector<std::pair<int, int64_t>> GetClassIDAndFileID(const std::string& s)
	{
		std::vector<std::pair<int, int64_t>> pairs;

		std::regex pattern(R"(--- !u!(\d+) &(\d+))");
		auto words_begin =
			std::sregex_iterator(s.begin(), s.end(), pattern);
		auto words_end = std::sregex_iterator();
		auto count = std::distance(words_begin, words_end);
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

	std::string RemoveStripped(const std::string& s)
	{
		std::regex pattern(R"((--- !u!\d+ &\d+) stripped)");
		std::string out = s;
		std::regex_replace(out.begin(), s.begin(), s.end(), pattern, "$1");
		return out;
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

	std::vector<Object*> YAMLInputArchive::LoadAllFromString(const std::string& str)
	{
		auto classID_fileID = GetClassIDAndFileID(str);
		auto str2 = RemoveStripped(str);
		m_nodes = YAML::LoadAll(str2);
		assert(m_nodes.size() == classID_fileID.size());

		for (auto&& node : m_nodes)
			PatchNode(node);

		std::vector<Object*> objects(m_nodes.size());

		// step 1: instantiate prefabs
		std::map<int64_t, Prefab*> fileIDToPrefab;
		for (int i = 0; i < m_nodes.size(); ++i)
		{
			int classID = classID_fileID[i].first;
			int64_t fileID = classID_fileID[i].second;
			if (classID == Prefab::ClassID)
			{
				Object* obj = nullptr;
				auto&& node = m_nodes[i];
				auto&& parentPrefab = node.begin()->second["m_ParentPrefab"];
				auto parentFileID = parentPrefab["fileID"].as<int64_t>();

				if (parentFileID == 0)	// this prefab is a definition
				{
//					obj = CreateEmptyObject<Prefab>();
					obj = nullptr;
				}
				else	// this prefab is a ref/instance
				{
					std::string guid = parentPrefab["guid"].as<std::string>();
					std::string assetPath = AssetDatabase::GUIDToAssetPath(guid);
					Prefab* prefab = dynamic_cast<Prefab*>( AssetDatabase::LoadMainAssetAtPath(assetPath));

					auto&& modificationNode = node.begin()->second["m_Modification"];
					PrefabModification modification;
					PushNode(modificationNode);
					(*this) >> modification;
					PopNode();

					//LogError("[TODO] make a copy of main asset");
					Prefab* instance  = prefab->InstantiateWithModification(modification);
					fileIDToPrefab[fileID] = instance;
					obj = instance->GetRootGameObject();
//					obj = instance;
				}
				objects[i] = obj;
				m_FileIDToObject[fileID] = obj;
			}
		}

		// step 2: clone all objects (except ref to prefab)
		for (int i = 0; i < m_nodes.size(); ++i)
		{
			auto&& node = m_nodes[i].begin()->second;
			int classID = classID_fileID[i].first;
			int64_t fileID = classID_fileID[i].second;
			
			Object* obj = nullptr;
			
			if (classID != Prefab::ClassID)
			{
				bool done = false;
				if (YAMLNodeHasKey(node, "m_PrefabParentObject"))
				{
					auto id1 = node["m_PrefabParentObject"]["fileID"].as<int64_t>();
					auto id2 = node["m_PrefabInternal"]["fileID"].as<int64_t>();
					if (id1 != 0 && id2 != 0)
					{
						auto prefab = fileIDToPrefab[id2];
						assert(prefab != nullptr);
						obj = prefab->GetObjectByFileID(id1);
						done = true;
					}
				}
				if (!done)
					obj = CreateEmptyObjectByClassID(classID);
			}
			else
			{
				obj = objects[i];
			}

			objects[i] = obj;
			m_FileIDToObject[fileID] = obj;
			if (obj != nullptr)
				obj->SetLocalIdentifierInFile(fileID);
		}

		// add component to gameObject
//		for (int i = 0; i < m_nodes.size(); ++i)
//		{
//			auto&& node = m_nodes[i].begin()->second;
//			int classID = classID_fileID[i].first;
//			int64_t fileID = classID_fileID[i].second;
//
//			if (classID == GameObject::ClassID)
//			{
//				if (YAMLNodeHasKey(node, "m_Component"))
//				{
//
//				}
//			}
//		}

		// deserialize GameObjects from archive
		for (int i = 0; i < m_nodes.size(); ++i)
		{
			auto&& node = m_nodes[i];
			assert(node.IsMap());
			auto className = node.begin()->first.as<std::string>();
			Object* obj = objects[i];
			int classID = classID_fileID[i].first;

			if (obj != nullptr && classID == GameObject::ClassID)
			{
				assert(className == obj->GetClassName());
				if (obj->As<GameObject>()->GetPrefabInternal() == nullptr)	// not a prefab instance
				{
					PushNode(node.begin()->second);
					obj->Deserialize(*this);
					PopNode();
				}
			}
		}


		// deserialize from archive
		for (int i = 0; i < m_nodes.size(); ++i)
		{
			auto&& node = m_nodes[i];
			assert(node.IsMap());
			auto className = node.begin()->first.as<std::string>();
			Object* obj = objects[i];
			int classID = classID_fileID[i].first;

			if (obj != nullptr && classID != GameObject::ClassID && classID != Prefab::ClassID)
			{
				assert(className == obj->GetClassName());
				PushNode(node.begin()->second);
				obj->Deserialize(*this);
				PopNode();
			}
		}
		return objects;
	}



	Object* YAMLInputArchive::DeserializeObject()
	{
		auto current = CurrentNode();
		int64_t fileID = current["fileID"].as<int64_t>();

		if (YAMLNodeHasKey(current, "guid"))	// extern fileID
		{
			auto guid = current["guid"].as<std::string>();
//			LogWarning(guid);
			return AssetDatabase::GetAssetByGUIDAndFileID(guid, fileID);
		}
		else {	// local fileID
			if (fileID != 0)
			{
				auto it = this->m_FileIDToObject.find(fileID);
				if (it != this->m_FileIDToObject.end())
					return it->second;
				else
					abort();
			}
		}

		return nullptr;
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
				int64_t fileID = o->GetLocalIdentifierInFile();
				if (fileID == 0)
				{
					LogWarning("Object fileID is 0");
					fileID = o->GetInstanceID();
				}

				done.insert(o);
				NewLine();
				fout << Format("--- !u!{} &{}\n", o->GetClassID(), fileID);
				beginOfLine = true;
				this->MapKey(o->GetClassName());
				o->Serialize(*this);
				this->AfterValue();
			}
		}
	}

	void YAMLOutputArchive::SerializeObject(Object* t)
	{
		
		std::string s;
		int64_t fileID = t->GetLocalIdentifierInFile();
		if (fileID == 0)
		{
			LogWarning("Object fileID is 0");
			fileID = t->GetInstanceID();
		}
		if (t->Is<Mesh>() || t->Is<Material>())
		{
			std::string guid = AssetDatabase::GetGUIDFromInstanceID(t->GetInstanceID());
			s = Format("{{fileID: {}, guid: {}}}", fileID, guid);
		}
		else
		{
			s = Format("{{fileID: {}}}", fileID);
			todo.push_back(t);
		}
		
		Serialize(s);
	}

}

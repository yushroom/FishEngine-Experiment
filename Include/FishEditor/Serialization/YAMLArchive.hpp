#pragma once

#include <FishEngine/Serialization/Archive.hpp>
#include <FishEngine/FishEngine2.hpp>

#include <set>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <regex>

#include <yaml-cpp/yaml.h>

using namespace FishEngine;

namespace FishEditor
{
    template<class T>
    inline T* CreateObject() { return new T(); }

    template<>
    inline GameObject* CreateObject<GameObject>() {
		return new GameObject("", GameObjectConstructionFlag::Empty);
	}

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
            int classID = std::stoi( match[1] );
            long long fileID = std::stoll ( match[2] );
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
			if (n["near clip plane"] and !n["m_NearClipPlane"])
				n["m_NearClipPlane"] = n["near clip plane"];
			if (n["far clip plane"] and !n["m_FarClipPlane"])
				n["m_FarClipPlane"] = n["far clip plane"];
			if (n["field of view"] and !n["m_FieldOfView"])
				n["m_FieldOfView"] = n["field of view"];
			if (n["orthographic"] and !n["m_Orthographic"])
				n["m_Orthographic"] = n["orthographic"];
			if (n["orthographic size"] and !n["m_OrthographicSize"])
				n["m_OrthographicSize"] = n["orthographic size"];
		}
	}
	

	class YAMLInputArchive : public InputArchive
	{
	public:
		YAMLInputArchive() = default;

		std::vector<Object*> LoadAll(const std::string& path)
		{
			std::ifstream is(path);
			assert(is.good());
			std::stringstream buffer;
			buffer << is.rdbuf();
			auto pairs = GetClassIDAndFileID(buffer.str());
			m_nodes = YAML::LoadAll(buffer.str());
			assert(m_nodes.size() == pairs.size());
			
			for (auto&& node : m_nodes)
				PatchNode(node);

			std::vector<Object*> objects(m_nodes.size());
			
#define E(T) \
			else if (classID == T::ClassID) \
				obj = CreateObject<T>();
			
			for (int i = 0; i < m_nodes.size(); ++i)
			{
				int classID = pairs[i].first;
				int fileID = pairs[i].second;
				Object* obj = nullptr;
				if (classID == GameObject::ClassID)
					obj = CreateObject<GameObject>();
				else if (classID == Transform::ClassID)
					obj = CreateObject<Transform>();
				E(Camera)
				E(Light)
				E(RectTransform)
				E(MeshFilter)
				E(MeshRenderer)
				E(BoxCollider)
				E(SphereCollider)
				E(Rigidbody)
				objects[i] = obj;
				m_FileIDToObject[fileID] = obj;
			}
#undef E
			
			for (int i = 0; i < m_nodes.size(); ++i)
			{
//				int classID = pairs[i].first;
//				int fileID = pairs[i].second;
				auto&& node = m_nodes[i];
				assert(node.IsMap());
//				auto className = node.begin()->first.as<std::string>();
				Object* obj = objects[i];
				if (obj != nullptr)
				{
					PushNode( node.begin()->second );
					obj->Deserialize(*this);
					PopNode();
				}
			}
			return objects;
		}
		
		virtual Object* ParseObjectRef() override
		{
			auto current = CurrentNode();
			uint64_t fileID = current["fileID"].as<uint64_t>();
			if (fileID != 0)
			{
				auto it = m_FileIDToObject.find(fileID);
				if (it != m_FileIDToObject.end())
					return it->second;
			}
			return nullptr;
		}

		virtual void Add(std::string& t) override
		{
			t = CurrentNode().as<std::string>();
		}
		virtual void Add(int& t) override
		{
			t = CurrentNode().as<int>();
		}
		virtual void Add(float& t) override
		{
			t = CurrentNode().as<float>();
		}
		virtual void Add(bool& t) override
		{
			t = CurrentNode().as<int>() == 1;
		}
//		virtual void Add(const char * t) {}		// required
//		virtual void AddNone() {}
//		virtual void AddObject(Object* t) {}

		// Map
		virtual void MapKey(const char* name) override
		{
			auto current = CurrentNode();
			assert(current.IsMap());
//			m_workingNodes.push( current[name] );
			auto node = current[name];
			if (!node)
			{
				LogError(Format("Key [{}] not found!", name));
				abort();
			}
			PushNode(node);
		}

		virtual void AfterValue() override
		{
			PopNode();
		}

		// Sequence
		virtual int BeginSequence() override
		{
			auto current = CurrentNode();
			assert(current.IsSequence());
			int size = std::distance(current.begin(), current.end());
            m_sequenceiterator = current.begin();
			return size;
		}
        virtual void BeginSequenceItem() override
        {
			PushNode(*m_sequenceiterator);
        }

        virtual void AfterSequenceItem() override
        {
			PopNode();
            m_sequenceiterator++;
        }
		virtual void EndSequence() override
		{
//			PopNode();
		}

		void PushNode(YAML::Node node)
		{
//			puts("push");
			m_workingNodes.push(node);
		}


		void PopNode()
		{
//			puts("pop");
			m_workingNodes.pop();
		}

		YAML::Node CurrentNode()
		{
			assert(!m_workingNodes.empty());
			return m_workingNodes.top();
		}

	protected:
		std::vector<YAML::Node> m_nodes;
//		std::vector<std::pair<int, int64_t>> m_ClassIDAndFileID:
		YAML::Node m_currentNode;
		std::stack<YAML::Node>  m_workingNodes;
        YAML::const_iterator		m_sequenceiterator;
		std::map<int, Object*> m_FileIDToObject;
	};

	class YAMLArchive : public OutputArchive
	{
	public:
		explicit YAMLArchive(std::ofstream& fout) : fout(fout)
		{
			fout << "%YAML 1.1\n%TAG !u! tag:unity3d.com,2011:\n";
		}

		void Add(int t) override {
			fout << t;
		}
		void Add(float t) override {
			fout << t;
		}
		void Add(bool t) override {
			fout << t;
		}
		void Add(const char* t) override {
			fout << t;
		}
		void Add(const Vector2& t) override {
			fout << Format("{{x:{}, y:{}}}", t.x, t.y);
		}
		void Add(const Vector3& t) override {
			fout << Format("{{x:{}, y:{}, z:{}}}", t.x, t.y, t.x);
		}
		void Add(const Vector4& t) override {
			fout << Format("{{x:{}, y:{}, z:{}, w:{}}}", t.x, t.y, t.x, t.w);
		}
		void Add(const Quaternion& t) override {
			fout << Format("{{x:{}, y:{}, z:{}, w:{}}}", t.x, t.y, t.x, t.w);
		}

		void BeforeKey() override
		{
//			fout << "\n";
			Indent();
		}
		void AfterKey() override
		{
			fout << ": ";
			indent += 2;
		}
		void AfterValue() override
		{
			indent -= 2;
			fout << "\n";
			assert(indent >= 0);
		}
		
		void BeginSequence(int size) override
		{
			if (size == 0)
				fout << "[]";
//			else
//				state = State::Sequence;
		}

		void NewSequenceItem() override
		{
			fout << "\n";
			indent -= 2;
			Indent();
			indent += 2;
			fout << "- ";
		}
		
		void EndSequence() override
		{
//			state = State::None;
		}


		void AddNone() override {
			fout << "{fileID: 0}";
		}

		void AddObject(Object* t) override
		{
			todo.push_back(t);
			std::string s = Format("{{fileID: {}}}", t->GetInstanceID());
			OutputArchive::Add(s);
		}

		void Dump(Object* obj)
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
					fout << Format("--- !u!{} &{}", o->GetClassID(), o->GetInstanceID());
					OutputArchive::Add(Format("\n{}:\n", o->GetClassName()));
					indent += 2;
					o->Serialize(*this);
					indent -= 2;
				}
			}
		}

		void Indent()
		{
			for (int i = 0; i < indent; ++i)
				fout << ' ';
		}

		std::vector<Object*> todo;
		std::set<Object*> done;
		std::ofstream& fout;
		int indent = 0;
		
//		enum class State
//		{
//			None,
//			Sequence,
//			Map,
//		};
//
//		State state = State::None;
	};



	class CloneArchive : public OutputArchive
	{
	public:
		void AddObject(Object* t) override
		{
			if (t != nullptr)
			{
				//auto it = m_Objects.find(t);
				//if (it == m_Objects.end())
				//{
				//	m_Objects.insert(t);
				//	t->Serialize(*this);
				//}
			}
		}

		Object* current = nullptr;
	};
}

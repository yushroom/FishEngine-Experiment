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
	class YAMLInputArchive : public InputArchive
	{
	public:
		YAMLInputArchive() = default;

		std::vector<Object*> LoadAll(const std::string& path);
		
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

		virtual void Get(std::string& t) override
		{
			t = CurrentNode().as<std::string>();
		}
		virtual void Get(int& t) override
		{
			t = CurrentNode().as<int>();
		}
		virtual void Get(float& t) override
		{
			t = CurrentNode().as<float>();
		}
		virtual void Get(bool& t) override
		{
			t = CurrentNode().as<int>() == 1;
		}

		// Map
		virtual void MapKey(const char* name) override
		{
			auto current = CurrentNode();
			assert(current.IsMap());
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

		Object* GetObjectByFileID(int64_t fileID)
		{
			return m_FileIDToObject[fileID];
		}

	protected:
		std::vector<YAML::Node>		m_nodes;
//		std::vector<std::pair<int, int64_t>> m_ClassIDAndFileID:
		YAML::Node					m_currentNode;
		std::stack<YAML::Node>		m_workingNodes;
		YAML::const_iterator		m_sequenceiterator;
		std::map<int64_t, Object*>	m_FileIDToObject;
	};



	class YAMLOutputArchive : public OutputArchive
	{
	public:
		explicit YAMLOutputArchive(std::ofstream& fout) : fout(fout)
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

		void Dump(Object* obj);

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

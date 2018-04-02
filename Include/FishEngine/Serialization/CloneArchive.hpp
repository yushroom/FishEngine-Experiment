#pragma once

#include <FishEngine/Serialization/Archive.hpp>
#include <set>
#include <vector>
#include <FishEngine/Prefab.hpp>

namespace FishEngine
{
	class CollectObjectsArchive : public OutputArchive
	{
	public:
		void Collect(Object* obj)
		{
			if (obj->Is<Prefab>())
				return;
			this->SerializeObject(obj);
		}

		void CollectPrefab(Prefab* prefab)
		{
			prefab->Serialize(*this);
		}

	protected:
		virtual void Serialize(short t) override {}
		virtual void Serialize(unsigned short t) override {}
		virtual void Serialize(int t) override {}
		virtual void Serialize(unsigned int t) override {}
		virtual void Serialize(long t) override {}
		virtual void Serialize(unsigned long t) override {}
		virtual void Serialize(long long t) override {}
		virtual void Serialize(unsigned long long t) override {}
		virtual void Serialize(float t) override {}
		virtual void Serialize(double t) override {}
		virtual void Serialize(bool t) override {}
		virtual void Serialize(std::string const & t) override {}
		virtual void SerializeNullPtr() override {}	// nullptr
		void SerializeObject(Object* t) override
		{
			auto it = m_Objects.find(t);
			if (it == m_Objects.end())
			{
				m_Objects.insert(t);
				t->Serialize(*this);
			}
		}

		void MapKey(const char* name) override {}

	public:
		std::set<Object*> m_Objects;
	};


	class CloneOutputArchive : public OutputArchive
	{
	public:
		void AssertEmpty()
		{
			assert(m_IntValues.empty());
			assert(m_UIntValues.empty());
			assert(m_FloatValues.empty());
			assert(m_StringValues.empty());
			assert(m_ObjectValues.empty());
			assert(m_MapKeys.empty());
		}

	protected:
		virtual void Serialize(short t) override { m_IntValues.push_back(t); }
		virtual void Serialize(unsigned short t) override { m_UIntValues.push_back(t); }
		virtual void Serialize(int t) override { m_IntValues.push_back(t); }
		virtual void Serialize(unsigned int t) override { m_UIntValues.push_back(t); }
		virtual void Serialize(long t) override { m_IntValues.push_back(t); }
		virtual void Serialize(unsigned long t) override { m_UIntValues.push_back(t); }
		virtual void Serialize(long long t) override { m_IntValues.push_back(t); }
		virtual void Serialize(unsigned long long t) override { m_UIntValues.push_back(t); }
		virtual void Serialize(float t) override { m_FloatValues.push_back(t); }
		virtual void Serialize(double t) override { m_FloatValues.push_back(t); }
		virtual void Serialize(bool t) override { m_IntValues.push_back(t ? 1 : 0); }
		virtual void Serialize(std::string const & t) override { m_StringValues.push_back(t); }
		virtual void SerializeNullPtr() override { m_ObjectValues.push_back(nullptr); }	// nullptr
		virtual void SerializeObject(Object* t) override { m_ObjectValues.push_back(t); };

		void MapKey(const char* name) override
		{
			m_MapKeys.push_back(name);
		}

		//// Map
		//virtual void AfterValue() {}

		//// Sequence
		virtual void BeginSequence(int size) override
		{
			m_SequenceSize.push_back(size);
		}
		//virtual void BeforeSequenceItem() {}
		//virtual void AfterSequenceItem() {}
		//virtual void EndSequence() {}

	public:
		std::deque<int64_t> m_IntValues;
		std::deque<uint64_t> m_UIntValues;
		std::deque<double> m_FloatValues;
		std::deque<std::string> m_StringValues;
		std::deque<Object*> m_ObjectValues;

		std::deque<const char*> m_MapKeys;

		std::deque<int> m_SequenceSize;
	};
	
	class CloneInputArchive : public InputArchive
	{
	public:

		CloneOutputArchive & values;
		std::map<Object*, Object*> & objectMemo;

		CloneInputArchive(CloneOutputArchive& values, std::map<Object*, Object*>& objectMemo)
			: values(values), objectMemo(objectMemo)
		{
		}

	protected:
		virtual void Deserialize(short & t) override { t = values.m_IntValues.front(); values.m_IntValues.pop_front(); }
		virtual void Deserialize(unsigned short & t) override { t = values.m_UIntValues.front(); values.m_UIntValues.pop_front(); }
		virtual void Deserialize(int & t) override { t = values.m_IntValues.front(); values.m_IntValues.pop_front(); }
		virtual void Deserialize(unsigned int & t) override { t = values.m_UIntValues.front(); values.m_UIntValues.pop_front(); }
		virtual void Deserialize(long & t) override { t = values.m_IntValues.front(); values.m_IntValues.pop_front(); }
		virtual void Deserialize(unsigned long & t) override { t = values.m_UIntValues.front(); values.m_UIntValues.pop_front(); }
		virtual void Deserialize(long long & t) override { t = values.m_IntValues.front(); values.m_IntValues.pop_front(); }
		virtual void Deserialize(unsigned long long & t) override { t = values.m_UIntValues.front(); values.m_UIntValues.pop_front(); }
		virtual void Deserialize(float & t) override { t = values.m_FloatValues.front(); values.m_FloatValues.pop_front(); }
		virtual void Deserialize(double & t) override { t = values.m_FloatValues.front(); values.m_FloatValues.pop_front(); }
		virtual void Deserialize(bool & t) override { t = values.m_IntValues.front(); values.m_IntValues.pop_front(); }
		virtual void Deserialize(std::string & t) override { t = values.m_StringValues.front(); values.m_StringValues.pop_front(); }

		virtual Object* DeserializeObject() override
		{
			auto obj = values.m_ObjectValues.front();
			values.m_ObjectValues.pop_front();
			if (obj != nullptr)
			{
				obj = this->objectMemo[obj];
			}
			return obj;
		}

		// Map
		virtual bool MapKey(const char* name) override {
			assert(values.m_MapKeys.front() == std::string(name));
			values.m_MapKeys.pop_front();
			return true;
		}
		virtual void AfterValue() override {}

		// Sequence
		virtual int BeginSequence() override {
			int size = values.m_SequenceSize.front();
			values.m_SequenceSize.pop_front();
			return size;
		}
		virtual void BeginSequenceItem() override {}
		virtual void AfterSequenceItem() override {}
		virtual void EndSequence() override {}
	};
}
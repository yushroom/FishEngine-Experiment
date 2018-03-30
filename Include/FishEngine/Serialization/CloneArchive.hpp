#pragma once

#include <FishEngine/Serialization/Archive.hpp>
#include <set>
#include <vector>

namespace FishEngine
{
	class CollectObjectsArchive : public OutputArchive
	{
	public:
		void SerializeObject(Object* t) override
		{
			auto it = m_Objects.find(t);
			if (it == m_Objects.end())
			{
				m_Objects.insert(t);
				t->Serialize(*this);
			}
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
		virtual void Serialize(const char* t) override {}
		virtual void SerializeNullPtr() override {}	// nullptr

	public:
		std::set<Object*> m_Objects;
	};



	class CloneArchive : public OutputArchive
	{
	public:
		void SerializeObject(FishEngine::Object* t) override
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
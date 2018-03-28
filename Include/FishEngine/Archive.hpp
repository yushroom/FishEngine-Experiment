#include "Object.hpp"
#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"
#include "Math/Vector4.hpp"
#include "Math/Quaternion.hpp"
//#include "Math/Matrix4x4.hpp"

#include "Util/StringFormat.hpp"

#include <set>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>

namespace FishEngine
{
	class Archive
	{
	public:
		template<class T>
		void AddNVP(const char* name, T&& t)
		{
			this->BeginNVP();
			this->AddName(name);
			this->Add(t);
			this->EndNVP();
		}

		template<typename T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
		void Add(T t)
		{
			this->Add(static_cast<std::underlying_type_t<T>>(t));
		}

		template<typename T>
		void Add(const std::vector<T>& t)
		{
			for (auto&& item : t)
				this->Add(item);
		}

		template<typename T>
		void Add(const std::list<T>& t)
		{
			for (auto&& item : t)
				this->Add(item);
		}

		void Add(Object* t)
		{
			if (t == nullptr)
				AddNone();
			else
				AddObject(t);
		}

		void Add(const std::string& t)
		{
			Add(t.c_str());
		}

		virtual void Add(int t) {}
		virtual void Add(float t) {}
		virtual void Add(bool t) {}
		virtual void Add(const char * t) {}		// required
		virtual void Add(const Vector2& t) {}
		virtual void Add(const Vector3& t) {}
		virtual void Add(const Vector4& t) {}
		virtual void Add(const Quaternion& t) {}
		virtual void AddName(const char* name) {};
		virtual void AddNone() {}
		virtual void AddObject(Object* t) {}
		virtual void BeginNVP() {}
		virtual void EndNVP() {}
	};

	class CollectObjectArchive : public Archive
	{
	public:
		virtual void AddObject(Object* t) override
		{
			auto it = m_Objects.find(t);
			if (it == m_Objects.end())
			{
				m_Objects.insert(t);
				t->Serialize(*this);
			}
		}

	protected:
		std::set<Object*> m_Objects;
	};

	class OutputArchive : public Archive
	{
	public:
		OutputArchive(std::ofstream& fout) : fout(fout)
		{

		}

		virtual void Add(int t) {
			fout << t;
		}
		virtual void Add(float t) {
			fout << t;
		}
		virtual void Add(bool t) {
			fout << t;
		}
		virtual void Add(const char* t) {
			fout << t;
		}
		virtual void Add(const Vector2& t) {
			fout << Format("{{x:{}, y:{}}}", t.x, t.y);
		}
		virtual void Add(const Vector3& t) {
			fout << Format("{{x:{}, y:{}, z:{}}}", t.x, t.y, t.x);
		}
		virtual void Add(const Vector4& t) {
			fout << Format("{{x:{}, y:{}, z:{}, w:{}}}", t.x, t.y, t.x, t.w);
		}
		virtual void Add(const Quaternion& t) {
			fout << Format("{{x:{}, y:{}, z:{}, w:{}}}", t.x, t.y, t.x, t.w);
		}

		virtual void BeginNVP() override
		{
			Indent();
			indent += 2;
		}

		virtual void EndNVP() override
		{
			fout << "\n";
			indent -= 2;
			assert(indent >= 0);
		}

		virtual void AddName(const char* name)
		{
			fout << name << ": ";
		};

		virtual void AddNone() {
			fout << "{fileID: 0}";
		}

		virtual void AddObject(Object* t)
		{
			todo.push_back(t);
			std::string s = Format("{{fileID: {}}}", t->GetInstanceID());
			Archive::Add(s);
			//auto it = done.find(t);
			//if (it == done.end())
			//{
			//	//done.insert(t);
			//	//Add("Object:\n");
			//	//indent += 2;
			//	//t->Serialize(*this);
			//}
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
					Add("\nObject:\n");
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
	};

	class CloneArchive : public Archive
	{
	public:
		virtual void AddObject(Object* t) override
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

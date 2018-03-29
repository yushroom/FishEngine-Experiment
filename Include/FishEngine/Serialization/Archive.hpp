#pragma once

#include "../Object.hpp"
#include "../Math/Vector2.hpp"
#include "../Math/Vector3.hpp"
#include "../Math/Vector4.hpp"
#include "../Math/Quaternion.hpp"
//#include "Math/Matrix4x4.hpp"
#include "../Util/StringFormat.hpp"

#include <set>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>

namespace FishEngine
{
	class InputArchive
	{
	public:
		template<class T>
		void AddNVP(const char* name, T& t)
		{
			this->MapKey(name);
			this->Get(t);
			this->AfterValue();
		}

		template<typename T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
		void Get(T& t)
		{
			std::underlying_type_t<T> v;
			this->Get(v);
			t = static_cast<T>(v);
		}

		template<typename T>
		void Get(std::vector<T>& t)
		{
			int size = BeginSequence();
			for (int i = 0; i < size; ++i)
			{
				T item = NewSequenceItem<T>();
				t.push_back(item);
			}
			EndSequence();
		}

		template<typename T>
		void Get(std::list<T>& t)
		{
			int size = BeginSequence();
			for (int i = 0; i < size; ++i)
			{
				T item = NewSequenceItem<T>();
				t.push_back(item);
			}
			EndSequence();
		}

		template<class T>
		void Get(T*& t)
		{
			static_assert(std::is_base_of<Object, T>::value, "T must be a Object");
			t = (T*)ParseObjectRef();
		}
		
		void Get(Vector2& t)
		{
			this->AddNVP("x", t.x);
			this->AddNVP("y", t.y);
		}
		void Get(Vector3& t)
		{
			this->AddNVP("x", t.x);
			this->AddNVP("y", t.y);
			this->AddNVP("z", t.z);
		}
		void Get(Vector4& t)
		{
			this->AddNVP("x", t.x);
			this->AddNVP("y", t.y);
			this->AddNVP("z", t.z);
			this->AddNVP("w", t.w);
		}
		void Get(Quaternion& t) {
			this->AddNVP("x", t.x);
			this->AddNVP("y", t.y);
			this->AddNVP("z", t.z);
			this->AddNVP("w", t.w);
		}

		// override these methods

		virtual void Get(std::string& t) = 0;
		virtual void Get(int& t) = 0;
		virtual void Get(float& t) = 0;
		virtual void Get(bool& t) = 0;

		virtual Object* ParseObjectRef() = 0;

		// Map
		virtual void MapKey(const char* name) = 0;
		virtual void AfterValue() = 0;

		// Sequence
		virtual int BeginSequence() = 0;		// return sequence size
		virtual void BeginSequenceItem() = 0;
		virtual void AfterSequenceItem() = 0;
		virtual void EndSequence() = 0;

		template<class T>
		T NewSequenceItem()
		{
			BeginSequenceItem();
			T value;
			this->Get(value);
			AfterSequenceItem();
			return value;
		}
	};

	
	class OutputArchive
	{
	public:
		template<class T>
		void AddNVP(const char* name, T&& t)
		{
			this->BeforeKey();
			this->Add(name);
			this->AfterKey();
			this->Add(t);
			this->AfterValue();
		}

		template<typename T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
		void Add(T t)
		{
			this->Add(static_cast<std::underlying_type_t<T>>(t));
		}

		template<typename T>
		void Add(const std::vector<T>& t)
		{
			BeginSequence(t.size());
			for (auto&& item : t) {
				NewSequenceItem();
				this->Add(item);
			}
			EndSequence();
		}

		template<typename T>
		void Add(const std::list<T>& t)
		{
			BeginSequence(t.size());
			for (auto&& item : t) {
				NewSequenceItem();
				this->Add(item);
			}
			EndSequence();
		}

		void Add(Object*& t)
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
		
		virtual void Add(const Vector2& t)
		{
			this->AddNVP("x", t.x);
			this->AddNVP("y", t.y);
		}
		virtual void Add(const Vector3& t) {
			this->AddNVP("x", t.x);
			this->AddNVP("y", t.y);
			this->AddNVP("z", t.z);
		}
		virtual void Add(const Vector4& t) {
			this->AddNVP("x", t.x);
			this->AddNVP("y", t.y);
			this->AddNVP("z", t.z);
			this->AddNVP("w", t.w);
		}
		virtual void Add(const Quaternion& t) {
			this->AddNVP("x", t.x);
			this->AddNVP("y", t.y);
			this->AddNVP("z", t.z);
			this->AddNVP("w", t.w);
		}

		virtual void Add(int t) {}
		virtual void Add(float t) {}
		virtual void Add(bool t) {}
		virtual void Add(const char * t) {}		// required

		virtual void AddNone() {}
		virtual void AddObject(Object* t) {}

		// Map
		virtual void BeforeKey() {}
		virtual void AfterKey() {}
		virtual void AfterValue() {}

		// Sequence
		virtual void BeginSequence(int size) {}
		virtual void NewSequenceItem() {}
		virtual void EndSequence() {}
	};
}

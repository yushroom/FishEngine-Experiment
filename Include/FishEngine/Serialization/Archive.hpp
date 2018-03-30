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

#include <type_traits>

template <typename T, typename = int>
struct HasSerialize : std::false_type { };

template <typename T>
struct HasSerialize <T, decltype((void) T::Serialize, 0)> : std::true_type { };

namespace FishEngine
{
	class InputArchive
	{
	public:
		template<class T>
		void AddNVP(const char* name, T& t)
		{
			this->MapKey(name);
			(*this) >> t;
			this->AfterValue();
		}
		
		InputArchive & operator >> (short & t)
		{
			this->Deserialize(t);
			return *this;
		}
		
		InputArchive & operator >> (unsigned short & t)
		{
			this->Deserialize(t);
			return *this;
		}
		
		InputArchive & operator >> (int & t)
		{
			this->Deserialize(t);
			return *this;
		}
		
		InputArchive & operator >> (unsigned int & t)
		{
			this->Deserialize(t);
			return *this;
		}
		
		InputArchive & operator >> (long & t)
		{
			this->Deserialize(t);
			return *this;
		}
		
		InputArchive & operator >> (unsigned long & t)
		{
			this->Deserialize(t);
			return *this;
		}
		
		InputArchive & operator >> (long long & t)
		{
			this->Deserialize(t);
			return *this;
		}
		
		InputArchive & operator >> (unsigned long long & t)
		{
			this->Deserialize(t);
			return *this;
		}
		
		InputArchive & operator >> (float & t)
		{
			this->Deserialize(t);
			return *this;
		}
		
		InputArchive & operator >> (double & t)
		{
			this->Deserialize(t);
			return *this;
		}
		
		InputArchive & operator >> (bool & t)
		{
			this->Deserialize(t);
			return *this;
		}
		
		InputArchive & operator >> (std::string & t)
		{
			this->Deserialize(t);
			return *this;
		}

		template<typename T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
		InputArchive& operator>>(T& t)
		{
			std::underlying_type_t<T> v;
			(*this) >> v;
			t = static_cast<T>(v);
			return *this;
		}

		template<typename T>
		InputArchive& operator>>(std::vector<T>& t)
		{
			int size = BeginSequence();
			for (int i = 0; i < size; ++i)
			{
				T item = GetSequenceItem<T>();
				t.push_back(item);
			}
			EndSequence();
			return *this;
		}

		template<typename T>
		InputArchive& operator>>(std::list<T>& t)
		{
			int size = BeginSequence();
			for (int i = 0; i < size; ++i)
			{
				T item = GetSequenceItem<T>();
				t.push_back(item);
			}
			EndSequence();
			return *this;
		}

		template<class T>
		InputArchive& operator>>(T*& t)
		{
			static_assert(std::is_base_of<Object, T>::value, "T must be a Object");
			t = (T*) GetObject();
			return *this;
		}

		
		// override these methods

		virtual void Deserialize(short & t) = 0;
		virtual void Deserialize(unsigned short & t) = 0;
		virtual void Deserialize(int & t) = 0;
		virtual void Deserialize(unsigned int & t) = 0;
		virtual void Deserialize(long & t) = 0;
		virtual void Deserialize(unsigned long & t) = 0;
		virtual void Deserialize(long long & t) = 0;
		virtual void Deserialize(unsigned long long & t) = 0;
		virtual void Deserialize(float & t) = 0;
		virtual void Deserialize(double & t) = 0;
		virtual void Deserialize(bool & t) = 0;
		virtual void Deserialize(std::string & t) = 0;

		virtual Object* GetObject() = 0;

		// Map
		virtual void MapKey(const char* name) = 0;
		virtual void AfterValue() = 0;

		// Sequence
		virtual int BeginSequence() = 0;		// return sequence size
		virtual void BeginSequenceItem() = 0;
		virtual void AfterSequenceItem() = 0;
		virtual void EndSequence() = 0;

		template<class T>
		T GetSequenceItem()
		{
			BeginSequenceItem();
			T value;
			(*this) >> value;
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
			Serialize(name);
			this->AfterKey();
			(*this) << t;
			this->AfterValue();
		}
		
		OutputArchive & operator << (short t)
		{
			this->Serialize(t);
			return *this;
		}
		
		OutputArchive & operator << (unsigned short t)
		{
			this->Serialize(t);
			return *this;
		}
		
		OutputArchive & operator << (int t)
		{
			this->Serialize(t);
			return *this;
		}
		
		OutputArchive & operator << (unsigned int t)
		{
			this->Serialize(t);
			return *this;
		}
		
		OutputArchive & operator << (long t)
		{
			this->Serialize(t);
			return *this;
		}
		
		OutputArchive & operator << (unsigned long t)
		{
			this->Serialize(t);
			return *this;
		}
		
		OutputArchive & operator << (long long t)
		{
			this->Serialize(t);
			return *this;
		}
		
		OutputArchive & operator << (unsigned long long t)
		{
			this->Serialize(t);
			return *this;
		}
		
		OutputArchive & operator << (float t)
		{
			this->Serialize(t);
			return *this;
		}
		
		OutputArchive & operator << (double t)
		{
			this->Serialize(t);
			return *this;
		}
		
		OutputArchive & operator << (bool t)
		{
			this->Serialize(t);
			return *this;
		}
		
		OutputArchive & operator << (std::nullptr_t const & t)
		{
			this->Serialize(t);
			return *this;
		}

		template<typename T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
		OutputArchive & operator << (T const & t)
		{
			(*this) << static_cast<std::underlying_type_t<T>>(t);
			return *this;
		}
		
		OutputArchive & operator << (std::string const & t)
		{
			this->Serialize(t);
			return *this;
		}
		
		OutputArchive & operator << (const char* t)
		{
			this->Serialize(t);
			return *this;
		}
		

		template<typename T>
		OutputArchive & operator << (const std::vector<T>& t)
		{
			BeginSequence(t.size());
			for (auto&& item : t) {
				NewSequenceItem();
				(*this) << item;
			}
			EndSequence();
			return (*this);
		}

		template<typename T>
		OutputArchive & operator << (const std::list<T>& t)
		{
			BeginSequence(t.size());
			for (auto&& item : t) {
				NewSequenceItem();
				(*this) << item;
			}
			EndSequence();
			return (*this);
		}

		OutputArchive & operator << (Object* t)
		{
			if (t == nullptr)
				Serialize(nullptr);
			else
				SerializeObject(t);
			return (*this);
		}
		
//		virtual void Add(const Vector2& t)
//		{
//			this->AddNVP("x", t.x);
//			this->AddNVP("y", t.y);
//		}
//		virtual void Add(const Vector3& t) {
//			this->AddNVP("x", t.x);
//			this->AddNVP("y", t.y);
//			this->AddNVP("z", t.z);
//		}
//		virtual void Add(const Vector4& t) {
//			this->AddNVP("x", t.x);
//			this->AddNVP("y", t.y);
//			this->AddNVP("z", t.z);
//			this->AddNVP("w", t.w);
//		}
//		virtual void Add(const Quaternion& t) {
//			this->AddNVP("x", t.x);
//			this->AddNVP("y", t.y);
//			this->AddNVP("z", t.z);
//			this->AddNVP("w", t.w);
//		}


	protected:
		virtual void Serialize(short t) = 0;
		virtual void Serialize(unsigned short t) = 0;
		virtual void Serialize(int t) = 0;
		virtual void Serialize(unsigned int t) = 0;
		virtual void Serialize(long t) = 0;
		virtual void Serialize(unsigned long t) = 0;
		virtual void Serialize(long long t) = 0;
		virtual void Serialize(unsigned long long t) = 0;
		virtual void Serialize(float t) = 0;
		virtual void Serialize(double t) = 0;
		virtual void Serialize(bool t) = 0;
		virtual void Serialize(std::string const & t) = 0;
		virtual void Serialize(const char* t) = 0;
		virtual void Serialize(std::nullptr_t const & t) = 0;
		
		virtual void SerializeObject(Object* t) = 0;


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

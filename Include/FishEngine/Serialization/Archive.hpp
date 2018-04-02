#pragma once

#include "../Object.hpp"
#include "../Math/Vector2.hpp"
#include "../Math/Vector3.hpp"
#include "../Math/Vector4.hpp"
#include "../Math/Quaternion.hpp"
//#include "Math/Matrix4x4.hpp"
#include "../Util/StringFormat.hpp"
#include "../Debug.hpp"

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

		InputArchive() = default;
		InputArchive(InputArchive&) = delete;
		InputArchive& operator=(InputArchive&) = delete;

		template<class T>
		void AddNVP(const char* name, T& t)
		{
			if (this->MapKey(name))
				(*this) >> t;
//			else
//				LogWarning(std::string("skip ") + name);
			this->AfterValue();
		}
		
		InputArchive & operator >> (short & t)				{ this->Deserialize(t); return *this; }
		InputArchive & operator >> (unsigned short & t)		{ this->Deserialize(t); return *this; }
		InputArchive & operator >> (int & t)				{ this->Deserialize(t); return *this; }
		InputArchive & operator >> (unsigned int & t)		{ this->Deserialize(t); return *this; }
		InputArchive & operator >> (long & t)				{ this->Deserialize(t); return *this; }
		InputArchive & operator >> (unsigned long & t)		{ this->Deserialize(t); return *this; }
		InputArchive & operator >> (long long & t)			{ this->Deserialize(t); return *this; }
		InputArchive & operator >> (unsigned long long & t) { this->Deserialize(t); return *this; }
		InputArchive & operator >> (float & t)				{ this->Deserialize(t); return *this; }
		InputArchive & operator >> (double & t)				{ this->Deserialize(t); return *this; }
		InputArchive & operator >> (bool & t)				{ this->Deserialize(t); return *this; }
		InputArchive & operator >> (std::string & t)		{ this->Deserialize(t); return *this; }

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
//			t.clear();
			t.resize(size);
			for (int i = 0; i < size; ++i)
			{
				T item = GetSequenceItem<T>();
				t[i] = item;
			}
			EndSequence();
			return *this;
		}

		template<typename T>
		InputArchive& operator>>(std::list<T>& t)
		{
			int size = BeginSequence();
			t.clear();
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
			t = (T*) DeserializeObject();
			return *this;
		}


	protected:
		// override these methods

		// if should skip next node, return true
//		virtual bool Skip() { return false; }

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

		virtual Object* DeserializeObject() = 0;

		// Map
		// if should skip next node, return false
		virtual bool MapKey(const char* name) = 0;
		virtual void AfterValue() {}

		// Sequence
		virtual int BeginSequence() = 0;		// return sequence size
		virtual void BeginSequenceItem() {}
		virtual void AfterSequenceItem() {}
		virtual void EndSequence() {}

	protected:
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

		OutputArchive() = default;
		OutputArchive(OutputArchive&) = delete;
		OutputArchive& operator=(OutputArchive&) = delete;


		template<class T>
		void AddNVP(const char* name, T&& t)
		{
			this->MapKey(name);
			(*this) << t;
			this->AfterValue();
		}

		OutputArchive & operator << (short t) { this->Serialize(t); return *this; }
		OutputArchive & operator << (unsigned short t) { this->Serialize(t); return *this; }
		OutputArchive & operator << (int t) { this->Serialize(t); return *this; }
		OutputArchive & operator << (unsigned int t) { this->Serialize(t); return *this; }
		OutputArchive & operator << (long t) { this->Serialize(t); return *this; }
		OutputArchive & operator << (unsigned long t) { this->Serialize(t); return *this; }
		OutputArchive & operator << (long long t) { this->Serialize(t); return *this; }
		OutputArchive & operator << (unsigned long long t) { this->Serialize(t); return *this; }
		OutputArchive & operator << (float t) { this->Serialize(t); return *this; }
		OutputArchive & operator << (double t) { this->Serialize(t); return *this; }
		OutputArchive & operator << (bool t) { this->Serialize(t); return *this; }
		OutputArchive & operator << (std::string const & t) { this->Serialize(t); return *this; }
		OutputArchive & operator << (const char* t) { this->Serialize(t); return *this; }
		OutputArchive & operator << (std::nullptr_t const & t) { this->SerializeNullPtr(); return *this; }

		// Enum
		template<typename T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
		OutputArchive & operator << (T const & t)
		{
			(*this) << static_cast<std::underlying_type_t<T>>(t);
			return *this;
		}

		// Vector
		template<typename T>
		OutputArchive & operator << (const std::vector<T>& t)
		{
			BeginSequence(t.size());
			for (auto&& item : t) {
				BeforeSequenceItem();
				(*this) << item;
				AfterSequenceItem();
			}
			EndSequence();
			return (*this);
		}

		// List
		template<typename T>
		OutputArchive & operator << (const std::list<T>& t)
		{
			BeginSequence(t.size());
			for (auto&& item : t) {
				BeforeSequenceItem();
				(*this) << item;
				AfterSequenceItem();
			}
			EndSequence();  
			return (*this);
		}

		// Object*
		OutputArchive & operator << (Object* t)
		{
			if (t == nullptr)
				SerializeNullPtr();
			else
				SerializeObject(t);
			return (*this);
		}

	protected:
		void Serialize(const char* t) { Serialize(std::string(t)); };


		// override these methods

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

		virtual void SerializeNullPtr() = 0;	// nullptr
		virtual void SerializeObject(Object* t) = 0;


		// Map
		virtual void MapKey(const char* name) = 0;
		virtual void AfterValue() {}

		// Sequence
		virtual void BeginSequence(int size) {}
		virtual void BeforeSequenceItem() {}
		virtual void AfterSequenceItem() {}
		virtual void EndSequence() {}
	};
}

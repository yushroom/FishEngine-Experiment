#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "FishEngine.hpp"
#include "HideFlags.hpp"

#include <pybind11/pybind11.h>
//#include <functional>	// hash

namespace FishEngine
{
	class Object
	{
	public:

		Object(int classID);
		virtual ~Object() = 0;
		
		// noncopyable
		Object(const Object&) = delete;
		const Object& operator=(const Object&) = delete;

	public:

		const std::string& GetName() const
		{
			return m_Name;
		}
		void SetName(const std::string& name)
		{
			m_Name = name;
		}

		int GetInstanceID() const
		{
			return m_InstanceID;
		}
		int GetClassID() const
		{
			return m_ClassID;
		}
		
		void SetPyObject(const pybind11::object& obj)
		{
			m_PyObject = obj;
		}
		
		const pybind11::object& GetPyObject() const
		{
			return m_PyObject;
		}

		HideFlags GetHideFlags() const { return m_ObjectHideFlags; }
		void SetHideFlags(HideFlags flags) { m_ObjectHideFlags = flags; }

		uint64_t GetLocalIdentifierInFile() const { return m_LocalIdentifierInFile; }
		void SetLocalIdentifierInFile(uint64_t value) { m_LocalIdentifierInFile = value; }

		
	public:
		static int GetInstanceCounter()
		{
			return s_InstanceCounter;
		}
		
		static int GetDeleteCounter()
		{
			return s_DeleteCounter;
		}
		
		template<class T>
		static T* FindObjectOfType()
		{
			auto& objs = s_Objects[T::ClassID];
			if (objs.empty())
				return nullptr;
			auto first = *objs.begin();
			return dynamic_cast<T*>(first);
		}

		
		template<class T>
		static const std::unordered_set<Object*>& FindObjectsOfType()
		{
			return s_Objects[T::ClassID];
		}
		
		static const std::unordered_set<Object*>& FindObjectsOfType(int classID)
		{
			return s_Objects[classID];
		}

		static const std::unordered_map<int, std::unordered_set<Object*>>& GetAllObjects()
		{
			return s_Objects;
		}
		
	protected:
		std::string			m_Name;
		pybind11::object	m_PyObject = pybind11::none();
		HideFlags			m_ObjectHideFlags = HideFlags::None;

	private:
		int					m_ClassID = 0;
		int					m_InstanceID = 0;
		uint64_t			m_LocalIdentifierInFile = 0;
		
	private:
		static int s_InstanceCounter;
		static int s_DeleteCounter;
		static std::unordered_map<int, std::unordered_set<Object*>> s_Objects;
	};



	inline Object::Object(int classID) : m_ClassID(classID)
	{
		++s_InstanceCounter;
		m_InstanceID = s_InstanceCounter;
//		printf("Object::Object() ID=%d\n", instanceID);
		s_Objects[classID].insert(this);
	}
	
	inline Object::~Object()
	{
//		LOGF;
//		printf("Object::~Object() ID=%d\n", m_instanceID);
		++s_DeleteCounter;
		Object::s_Objects[m_ClassID].erase(this);
	}

}


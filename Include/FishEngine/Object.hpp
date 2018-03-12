#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "FishEngine.hpp"

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
			return m_name;
		}
		void SetName(const std::string& name)
		{
			m_name = name;
		}

		int GetInstanceID() const
		{
			return m_instanceID;
		}
		int GetClassID() const
		{
			return m_classID;
		}
		
		void SetPyObject(const pybind11::object& obj)
		{
			m_self = obj;
		}
		
		const pybind11::object& GetPyObject() const
		{
			return m_self;
		}

		
	public:
		static int GetInstanceCounter()
		{
			return s_instanceCounter;
		}
		
		static int GetDeleteCounter()
		{
			return s_deleteCounter;
		}
		
		template<class T>
		static T* FindObjectOfType()
		{
			auto& objs = s_objects[T::ClassID];
			if (objs.empty())
				return nullptr;
			auto first = *objs.begin();
			return dynamic_cast<T*>(first);
		}

		
		template<class T>
		static const std::unordered_set<Object*>& FindObjectsOfType()
		{
			return s_objects[T::ClassID];
		}
		
		static const std::unordered_set<Object*>& FindObjectsOfType(int classID)
		{
			return s_objects[classID];
		}

		static const std::unordered_map<int, std::unordered_set<Object*>>& GetAllObjects()
		{
			return s_objects;
		}
		
	protected:
		std::string			m_name;
		pybind11::object	m_self = pybind11::none();

	private:
		int					m_classID = 0;
		int					m_instanceID = 0;
		
	private:
		static int s_instanceCounter;
		static int s_deleteCounter;
		static std::unordered_map<int, std::unordered_set<Object*>> s_objects;
	};
	
	inline Object::Object(int classID) : m_classID(classID)
	{
		++s_instanceCounter;
		m_instanceID = s_instanceCounter;
//		printf("Object::Object() ID=%d\n", instanceID);
		s_objects[classID].insert(this);
	}
	
	inline Object::~Object()
	{
//		LOGF;
//		printf("Object::~Object() ID=%d\n", m_instanceID);
		++s_deleteCounter;
		Object::s_objects[m_classID].erase(this);
	}

}


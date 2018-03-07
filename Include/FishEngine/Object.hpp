#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "FishEngine.hpp"

#include <pybind11/pybind11.h>
//#include <functional>	// hash

namespace FishEngine
{
//	template<class T>
//	constexpr int ClassID()
//	{
////		static_assert(std::is_integral<T::ClassID>::value, "ClassID<T> not implemented");
//		return T::ClassID;
//	}
	
    class Object
    {
    public:

		Object(int classID);
		virtual ~Object() = 0;
		
		// noncopyable
		Object(const Object&) = delete;
		const Object& operator=(const Object&) = delete;

        std::string name;
        int instanceID;

		int GetClassID() const
		{
			return m_classID;
		}
		
		void SetObject(const pybind11::object& obj)
		{
			LOGF;
			m_self = obj;
		}
		
		pybind11::object GetPyObject() const
		{
			return m_self;
		}

		
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
        static int s_instanceCounter;
		static int s_deleteCounter;
		
		int m_classID = 0;
		pybind11::object m_self;
		//pybind11::handle m_self;
		
	private:
		static std::unordered_map<int, std::unordered_set<Object*>> s_objects;
	};
	
	inline Object::Object(int classID) : m_classID(classID)
	{
		++s_instanceCounter;
		instanceID = s_instanceCounter;
//		printf("Object::Object() ID=%d\n", instanceID);
		s_objects[classID].insert(this);
	}
	
	inline Object::~Object()
	{
//		LOGF;
		printf("Object::~Object() ID=%d\n", instanceID);
		++s_deleteCounter;
		Object::s_objects[m_classID].erase(this);
	}

}

//namespace std {
//	template<>
//	struct hash<FishEngine::Object>
//	{
//		size_t operator()(const FishEngine::Object& o) const {
//			return o.instanceID;
//		}
//	};
//}


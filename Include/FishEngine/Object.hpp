#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "FishEngine.hpp"
#include <Python.h>
//#include <functional>	// hash

//struct PyObject;

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

		Object(PyObject* pyobj = nullptr);
		virtual ~Object() = 0;
		
		// noncopyable
		Object(const Object&) = delete;
		const Object& operator=(const Object&) = delete;

        std::string name;
        int instanceID;
		
		
//		static void Destroy(Object* obj)
//		{
//			delete obj;
//		}
		
		void SetObject(PyObject* obj)
		{
			LOGF;
			m_self = obj;
		}
		
		PyObject* GetPyObject() const
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
		
    protected:
        static int s_instanceCounter;
		static int s_deleteCounter;
		
		PyObject* m_self = nullptr;
		
	public:
		static std::unordered_map<int, std::unordered_set<Object*>> s_objects;
    };
	
	inline Object::Object(PyObject* pyobj) : m_self(pyobj)
	{
		++s_instanceCounter;
		instanceID = s_instanceCounter;
//		printf("Object::Object() ID=%d\n", instanceID);
	}
	
	inline Object::~Object()
	{
//		LOGF;
//		printf("Object::~Object() ID=%d\n", instanceID);
		++s_deleteCounter;
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


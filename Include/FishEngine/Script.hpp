#pragma once

#include "Component.hpp"

#include <boost/python.hpp>

#if 1
#define Func(name) \
    void name() \
    { \
		assert(m_self != nullptr); \
        boost::python::call_method<void>(m_self, #name); \
    }

// struct PyObject*;
namespace FishEngine
{
    // wrapper of python Script
    class Script : public Component
    {
    public:
		Script()
		{
			LOGF;
		}
		
        // Script(PyObject* o) : self(o) { }
        virtual ~Script()
        {
            LOGF;
        }

        Func(Awake)
        Func(Start)
        Func(Update)
        Func(FixedUpdate)
        Func(LateUpdate)
        Func(OnGUI)
        Func(OnDisable)
        Func(OnEnable)


		
//		PyObject* GetObject()
//		{
//			return self;
//		}
		
		virtual bool IsScript() const override final
		{
			return true;
		}
    };
}

#undef Func
#else

using namespace boost::python;

namespace FishEngine
{
	class Script : public Component
	{
	public:
		
		Script()
		{
			LOGF;
		}
		
		virtual ~Script()
		{
			LOGF;
		}
		
		virtual void Start()
		{
			LOGF;
		}
		
		virtual void Update()
		{
			LOGF;
		}
	};
	
	class ScriptWrap : public Script, public wrapper<Script>
	{
	public:
		virtual void Start() override
		{
			LOGF;
			if (override n = this->get_override("Start"))
				n();
//			else
//				Component::Start();
		}
		
		virtual void Update() override
		{
			LOGF;
			if (override n = this->get_override("Update"))
				n();
//			else
//				Component::Update();
		}
		
		void default_Start()
		{
			LOGF;
		}
		
		void default_Update()
		{
			LOGF;
		}
	};
}
#endif

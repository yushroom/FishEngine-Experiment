#pragma once

#include "Component.hpp"

#include <pybind11/pybind11.h>

#define Func(name) void name() { m_self.attr(#name)(); }

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

        Func(Awake)
        Func(Start)
        Func(Update)
        Func(FixedUpdate)
        Func(LateUpdate)
        Func(OnGUI)
        Func(OnDisable)
        Func(OnEnable)
		
		virtual bool IsScript() const override final
		{
			return true;
		}
    };
}

#undef Func

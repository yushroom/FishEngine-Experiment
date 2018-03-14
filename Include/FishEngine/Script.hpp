#pragma once

#include "Component.hpp"

#include <pybind11/pybind11.h>

#define Func(name) void name() { m_PyObject.attr(#name)(); }

namespace FishEngine
{
	class Script : public Component
	{
	public:
		enum { ClassID = 115 };
		Script() : Component(Script::ClassID)
		{
			LOGF;
		}

		virtual ~Script()
		{
			LOGF;
		}

		Func(Awake);
		Func(Start);
		Func(Update);
		Func(FixedUpdate);
		Func(LateUpdate);
		Func(OnGUI);
		Func(OnDisable);
		Func(OnEnable);

		virtual Script* Clone() const override;
	};
}

#undef Func

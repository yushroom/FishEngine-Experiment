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

		void SetPyObject(const pybind11::object& obj)
		{
			m_PyObject = obj;
		}

		const pybind11::object& GetPyObject() const
		{
			return m_PyObject;
		}

	protected:
		pybind11::object	m_PyObject = pybind11::none();
	};
}

#undef Func

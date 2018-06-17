#pragma once

#include "Component/Behaviour.hpp"
#include <pybind11/pybind11.h>

namespace FishEngine
{
	class Script : public Behaviour
	{
	public:
		DeclareObject(Script, 115);

		Script() : Behaviour(Script::ClassID, ClassName)
		{
			LOGF;
			s_LastScript = this;
		}
		
		virtual ~Script()
		{
			puts("Script::~Script");
			m_Script = pybind11::none();
		}
		
		void Start() { m_Script.attr("Start")(); }
		void Update() { m_Script.attr("Update")(); }
		void OnDrawGizmos() { m_Script.attr("OnDrawGizmos")(); }
//		Script* Clone() { return nullptr; }
		Script* Clone()
		{
			m_Script.attr("Clone")();
			return s_LastScript;
		}
		
		void SetPyObject(const pybind11::object& obj)
		{
			m_Script = obj;
		}
		
		const pybind11::object& GetPyObject() const
		{
			return m_Script;
		}
		
	protected:
		void CallMethod(const std::string& methodName)
		{
			m_Script.attr(methodName.c_str())();
		}
		
		pybind11::object	m_Script = pybind11::none();
		
		inline static Script* s_LastScript = nullptr;
	};
}

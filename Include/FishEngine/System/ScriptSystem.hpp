#pragma once

//#include <boost/python/object.hpp>

namespace FishEngine
{
	class ScriptSystem
	{
	public:
		static ScriptSystem& GetInstance()
		{
			static ScriptSystem instance;
			return instance;
		}
		
		void Init();
		void Update();
		void Clean();
		void Reload();
		
	protected:
		ScriptSystem() = default;
		
//	private:
//		boost::python::object m_appModule;
	};
}

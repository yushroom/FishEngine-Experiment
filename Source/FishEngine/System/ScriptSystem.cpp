#include <FishEngine/System/ScriptSystem.hpp>
#include <FishEditor/FishEditor.hpp>

#include <boost/python.hpp>
using namespace boost::python;

#include <cstdlib>  // setenv

#if PY_MAJOR_VERSION >= 3
#   define INIT_MODULE PyInit_FishEngineInternal
extern "C" PyObject* INIT_MODULE();
#else
#   define INIT_MODULE initFishEngineInternal
extern "C" void INIT_MODULE();
#endif

#if FISHENGINE_PLATFORM_WINDOWS
// https ://stackoverflow.com/questions/17258029/c-setenv-undefined-identifier-in-visual-studio
int setenv(const char *name, const char *value, int overwrite)
{
	int errcode = 0;
	if (!overwrite) {
		size_t envsize = 0;
		errcode = getenv_s(&envsize, NULL, 0, name);
		if (errcode || envsize) return errcode;
	}
	return _putenv_s(name, value);
}
#endif

namespace FishEngine
{
	static object s_appModule;
	
	void ScriptSystem::Init()
	{
#if FISHENGINE_PLATFORM_WINDOWS
		setenv("PYTHONPATH", R"(D:\program\FishEngine-Experiment\Script)", 1);
#else
		setenv("PYTHONPATH", "/Users/yushroom/program/FishEngine-Experiment/Script", 1);
#endif
		PyImport_AppendInittab((char*)"FishEngineInternal", INIT_MODULE);
		Py_Initialize();
		object main_module = import("__main__");
//		dict main_namespace = extract<dict>(main_module.attr("__dict__"));
		// object fei_module = import("FishEngineInternal");
		// main_namespace["precreated_object"] = Base("created on C++ side");
		// exec_file("testFishEngine.py", main_namespace, main_namespace);
		//        FishEngine::Run();
		{
			s_appModule = import("app");
			auto Start = s_appModule.attr("Start");
			Start();
		}
	}
	
	void ScriptSystem::Update()
	{
		auto Update = s_appModule.attr("Update");
		Update();
	}
	
	
	void ScriptSystem::Clean()
	{
		s_appModule.attr("Clean")();
	}
	
	void ScriptSystem::Reload()
	{
		Clean();
		s_appModule.attr("Reload")();
		Init();
	}
}

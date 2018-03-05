#include <FishEditor/FishEditorInternal.hpp>
#include "FBXImporter.cpp"

#include <boost/python.hpp>

using namespace FishEditor;


BOOST_PYTHON_MODULE(FishEditorInternal)
{
	using namespace boost::python;
	
	class_<FBXImporter, boost::noncopyable>("FBXImporter")
		.def("Create", &FBXImporter::Create, return_value_policy<manage_new_object>())
		.def("Import", &FBXImporter::Import)
		.def("GetMeshByName", &FBXImporter::GetMeshByName, return_internal_reference<>())
		.def_readwrite("globalScale", &FBXImporter::m_globalScale)
	;
}

#if PY_MAJOR_VERSION >= 3
#   define INIT_MODULE PyInit_FishEditorInternal
extern "C" PyObject* INIT_MODULE();
#else
#   define INIT_MODULE initFishEditorInternal
extern "C" void INIT_MODULE();
#endif

void FishEditor::Init()
{
	PyImport_AppendInittab("FishEditorInternal", INIT_MODULE);
}

#include <FishEditor/FishEditorInternal.hpp>
#include "FBXImporter.cpp"

#include <pybind11/embed.h>

using namespace FishEditor;


PYBIND11_EMBEDDED_MODULE(FishEditorInternal, m)
{
	using namespace pybind11;
	
	class_<FBXImporter>(m, "FBXImporter")
		.def(init<>())
		.def("Import", &FBXImporter::Import)
		.def("GetMeshByName", &FBXImporter::GetMeshByName, return_value_policy::reference)
		.def_readwrite("globalScale", &FBXImporter::m_globalScale)
	;
}

void FishEditor::Init()
{
}

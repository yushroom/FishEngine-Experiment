#include <FishEditor/FishEditorInternal.hpp>
#include <FishEditor/FBXImporter.hpp>

#include <pybind11/embed.h>

using namespace FishEditor;


PYBIND11_EMBEDDED_MODULE(FishEditorInternal, m)
{
	using namespace pybind11;
	
	class_<FBXImporter>(m, "FBXImporter")
		.def(init<>())
		.def("Import", &FBXImporter::Import)
//		.def("GetMeshByName", &FBXImporter::GetMeshByName, return_value_policy::reference)
		.def("GetObjectByFileID", &FBXImporter::GetObjectByFileID, return_value_policy::reference)
		.def("GetGlobalScale", &FBXImporter::GetGlobalScale)
		.def("SetGlobalScale", &FBXImporter::SetGlobalScale)
	;
}

void FishEditor::Init()
{
}

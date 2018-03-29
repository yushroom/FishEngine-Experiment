#include <FishEditor/FishEditorInternal.hpp>
#include <FishEditor/FBXImporter.hpp>
#include <FishEngine/GameObject.hpp>

#include <pybind11/embed.h>
#include <FishEditor/EditorApplication.hpp>
#include <FishEditor/AssetDatabase.hpp>

using namespace FishEditor;
namespace py = pybind11;

//py::dict FBXImporter_FileIDToObject(const FBXImporter& importer)
//{
//	py::dict ret;
//	auto&& d = importer.GetFileIDToObject();
//	for (auto&& p : d)
//	{
//		ret[py::cast(p.first)] = py::cast( p.second, py::return_value_policy::reference);
//	}
//	return ret;
//}

FBXImporter* CreateFBXImporter()
{
	return new FBXImporter;
}

void EditorApplication_OpenProject(const std::string& projectPath)
{
	EditorApplication::GetInstance().OpenProject(projectPath);
}


PYBIND11_EMBEDDED_MODULE(FishEditorInternal, m)
{
	using namespace pybind11;

	//m.def("FBXImporter_FileIDToObject", &FBXImporter_FileIDToObject);

	m.def("EditorApplication_OpenProject", &EditorApplication_OpenProject);

	class_<AssetDatabase>(m, "AssetDatabase")
		.def_static("GetAssetPathFromInstanceID", &AssetDatabase::GetAssetPathFromInstanceID)
		.def_static("", &AssetDatabase::AssetPathToGUID)
		.def_static("", &AssetDatabase::GUIDToAssetPath)
	;

	class_<AssetImporter, FishEngine::Object>(m, "AssetImporter")
		.def_static("AddImporter", &AssetImporter::AddImporter)
	;

	class_<ModelImporter, AssetImporter>(m, "ModelImporter")
		.def("GetGlobalScale", &ModelImporter::GetGlobalScale)
		.def("SetGlobalScale", &ModelImporter::SetGlobalScale)
		.def("GetUseFileScale", &ModelImporter::GetUseFileScale)
		.def("SetUseFileScale", &ModelImporter::SetUseFileScale)
		.def("GetFileScale", &ModelImporter::GetFileScale)
		.def("SetFileScale", &ModelImporter::SetFileScale)
	;
	

	m.def("CreateFBXImporter", CreateFBXImporter, return_value_policy::reference);
	class_<FBXImporter, ModelImporter>(m, "FBXImporter")
		.def(init<>())
		.def("Import", &FBXImporter::Import)
		.def("GetObjectByFileID", &FBXImporter::GetObjectByFileID, return_value_policy::reference)
		.def("GetRootGameObject", &FBXImporter::GetRootGameObject, return_value_policy::reference)
		.def("UpdateFileID", &FBXImporter::UpdateFileID)
	;
}

void FishEditor::Init()
{
}

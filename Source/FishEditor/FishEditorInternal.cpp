#include <FishEditor/FishEditorInternal.hpp>
#include <FishEditor/FBXImporter.hpp>
#include <FishEditor/Serialization/DefaultImporter.hpp>
#include <FishEditor/Serialization/NativeFormatImporter.hpp>
#include <FishEditor/EditorSceneManager.hpp>

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
		.def_static("AssetPathToGUID", &AssetDatabase::AssetPathToGUID)
		.def_static("GUIDToAssetPath", &AssetDatabase::GUIDToAssetPath)
	;

	class_<AssetImporter, FishEngine::Object>(m, "AssetImporter")
		//.def_static("AddImporter", &AssetImporter::AddImporter)
		.def_property("assetPath", &AssetImporter::GetAssetPath, &AssetImporter::SetAssetPath)
		.def_property("gUID", &AssetImporter::GetGUID, &AssetImporter::SetGUID)
		.def_property("assetTimeStamp", &AssetImporter::GetAssetTimeStamp, &AssetImporter::SetAssetTimeStamp)
		.def("GetAtPath", &AssetImporter::GetAtPath, return_value_policy::reference)
		.def("Import", &AssetImporter::Import)
	;

	class_<NativeFormatImporter, AssetImporter>(m, "NativeFormatImporter");
	class_<DefaultImporter, AssetImporter>(m, "DefaultImporter");

	class_<ModelImporter, AssetImporter>(m, "ModelImporter")
		.def("GetGlobalScale", &ModelImporter::GetGlobalScale)
		.def("SetGlobalScale", &ModelImporter::SetGlobalScale)
		.def("GetUseFileScale", &ModelImporter::GetUseFileScale)
		.def("SetUseFileScale", &ModelImporter::SetUseFileScale)
		.def("GetFileScale", &ModelImporter::GetFileScale)
	;
	

	m.def("CreateFBXImporter", CreateFBXImporter, return_value_policy::reference);
	class_<FBXImporter, ModelImporter>(m, "FBXImporter")
		.def(init<>())
		.def("Import", &FBXImporter::Import)
		.def("GetObjectByFileID", &FBXImporter::GetObjectByFileID, return_value_policy::reference)
		.def("GetRootGameObject", &FBXImporter::GetRootGameObject, return_value_policy::reference)
		.def("UpdateFileID", &FBXImporter::UpdateFileID)
	;

	py::enum_<OpenSceneMode>(m, "OpenSceneMode")
		.value("Single", OpenSceneMode::Single)
		.value("Additive", OpenSceneMode::Additive)
		.value("AdditiveWithoutLoading", OpenSceneMode::AdditiveWithoutLoading)
		.export_values();


	class_<EditorSceneManager>(m, "EditorSceneManager")
		.def("OpenScene", &EditorSceneManager::OpenScene, return_value_policy::reference)
	;
}

void FishEditor::Init()
{
}

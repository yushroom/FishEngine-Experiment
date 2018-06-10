#include <FishEngine/System/ScriptSystem.hpp>
#include <FishEditor/FishEditor.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
namespace py = pybind11;


#include <FishEngine/FishEngine2.hpp>
#include <FishEngine/FishEngine.hpp>
#include <FishEngine/Math/Vector2.hpp>
#include <FishEngine/Math/Vector3.hpp>
#include <FishEngine/Math/Quaternion.hpp>
#include <FishEngine/Math/Matrix4x4.hpp>
//#include <FishEngine/Object.hpp>
//#include <FishEngine/Prefab.hpp>
//#include <FishEngine/GameObject.hpp>
//#include <FishEngine/Transform.hpp>
//#include <FishEngine/RectTransform.hpp>
#include <FishEngine/Script.hpp>

#include <FishEngine/Render/Mesh.hpp>
#include <FishEngine/Render/Shader.hpp>
#include <FishEngine/Render/Material.hpp>


#include <FishEngine/Gizmos.hpp>
#include <FishEngine/Render/Graphics.hpp>
#include <FishEngine/Screen.hpp>
#include <FishEngine/Scene.hpp>

#include <pybind11/embed.h>
#include <pybind11/operators.h>



using namespace FishEngine;
namespace py = pybind11;


Object* FindObjectOfType(int classID)
{
	auto& objs = Object::FindObjectsOfType(classID);
	if (objs.size() == 0)
		return nullptr;
	return *objs.begin();
}

template<typename T>
inline py::list StdVector2PyList(const std::vector<T>& v)
{
	py::list ret;
	for (auto&& e : v)
	{
		ret.append(e);
	}
	return ret;
}

py::list FindObjectsOfType(int classID)
{
	auto& objs = Object::FindObjectsOfType(classID);
	py::list ret;
	for (auto o : objs)
	{
		ret.append(o);
	}
	return ret;
}

//py::object GameObject_GetComopnent(GameObject* go, int classID)
//{
//	assert(go != nullptr);
//	auto c = go->GetComponent(classID);
//	if (c == nullptr)
//		return py::none();
//	else
//		return c;
//}

py::list GameObject_GetAllComponents(GameObject* go)
{
	assert(go != nullptr);
	py::list ret;
	for (auto comp : go->GetAllComponents())
	{
		ret.append(comp);
	}
	return ret;
}

void Transform_GetChildren(Transform* t, py::list& outChildren)
{
	assert(t != nullptr);
	for (auto c : t->GetChildren())
	{
		outChildren.append(c);
	}
}

void Scene_GetRootGameObjects(Scene* scene, py::list& out)
{
	for (auto t : scene->GetRootTransforms())
	{
		out.append(t->GetGameObject());
	}
}

py::list SkinnedMeshRenderer__GetBones(SkinnedMeshRenderer* smr)
{
	return StdVector2PyList(smr->GetBones());
}


class PyScript : public Script
{
public:
	void Start() override
	{
		PYBIND11_OVERLOAD(void, Script, Start,);
	}
	
	void Update() override
	{
		PYBIND11_OVERLOAD(void, Script, Update,);
	}
	
	void OnDrawGizmos() override
	{
		PYBIND11_OVERLOAD(void, Script, OnDrawGizmos,);
	}
};


#define DefObject(classname) \
	m.def("Create"##classname, [](){ return new classname();}, return_value_policy::reference); \
	m.def(#classname"ClassID", []() { return classname::ClassID;  })

using namespace pybind11;

PYBIND11_EMBEDDED_MODULE(FishEngineInternal, m)
{
	m.def("FindObjectOfType", &FindObjectOfType);
	m.def("FindObjectsOfType", &FindObjectsOfType);

	// Vector2
	class_<Vector2>(m, "Vector2")
		.def(init<>())
		.def(init<float, float>())
		.def_readwrite("x", &Vector2::x)
		.def_readwrite("y", &Vector2::y)
		.def("Set", &Vector2::Set)
		.def(self + Vector2())
		.def(self - Vector2())
		.def(self*float())
		.def(float()*self)
		;

	// Vector3
	class_<Vector3>(m, "Vector3")
	.def(init<>())
	.def(init<float, float, float>())
	.def_readwrite("x", &Vector3::x)
	.def_readwrite("y", &Vector3::y)
	.def_readwrite("z", &Vector3::z)
	.def("Set", &Vector3::Set)
	.def(self + Vector3())
	.def(self - Vector3())
	.def(self*Vector3())
	.def(self*float())
	.def(float()*self)
	.def_static("Dot", &Vector3::Dot)
	.def_readonly_static("zero", &Vector3::zero)
	.def_readonly_static("one", &Vector3::one)
	.def_readonly_static("forward", &Vector3::forward)
	.def_readonly_static("up", &Vector3::up)
	.def_readonly_static("down", &Vector3::down)
	.def_readonly_static("back", &Vector3::back)
	.def_readonly_static("left", &Vector3::left)
	.def_readonly_static("right", &Vector3::right)
//		.def(py::pickle(
//					[](const Vector3 & v) { // __getstate__
//						return py::make_tuple(v.x, v.y, v.z);
//					},
//					[](py::tuple t) {
//						if (t.size() != 3)
//							throw std::runtime_error("Invalid state!");
//						Vector3 v(t[0].cast<float>(), t[1].cast<float>(), t[2].cast<float>());
//						return v;
//					}
//					))
		;


	// Quaternion
	class_<Quaternion>(m, "Quaternion")
		.def(init<>())
		.def(init<float, float, float, float>())
		.def_readwrite("x", &Quaternion::x)
		.def_readwrite("y", &Quaternion::y)
		.def_readwrite("z", &Quaternion::z)
		.def_readwrite("w", &Quaternion::w)
		.def("Set", &Quaternion::Set)
		.def_static("LookRotation", &Quaternion::LookRotation)
		//		.add_property("eulerAngles", &Quaternion::GetEulerAngles, &Quaternion::SetEulerAngles)
		;

	// Matrix4x4
	class_<Matrix4x4>(m, "Matrix4x4")
		.def("__str__", &Matrix4x4::ToString)
		.def(self*Matrix4x4())
		.def_static("Perspective", &Matrix4x4::Perspective)
		.def_static("LookAt", &Matrix4x4::LookAt)
		.def("MultiplyPoint", &Matrix4x4::MultiplyPoint)
		;

	class_<Color>(m, "Color")
	.def(init<>())
	.def(init<float, float, float>())
	.def(init<float, float, float, float>())
	.def_readwrite("r", &Color::r)
	.def_readwrite("g", &Color::g)
	.def_readwrite("b", &Color::b)
	.def_readwrite("a", &Color::a)
	.def_readonly_static("red", &Color::red)
	.def_readonly_static("green", &Color::green)
	;
	
	enum_<HideFlags>(m, "HideFlags")
		.value("None", HideFlags::None)
	;

	// Object
	class_<Object>(m, "Object")
		.def_property("name", &Object::GetName, &Object::SetName, return_value_policy::copy)
		.def_property_readonly("instanceID", &Object::GetInstanceID)
		.def("GetInstanceID", &Object::GetInstanceID)
		.def_property("localIdentifierInFile", &Object::GetLocalIdentifierInFile, &Object::SetLocalIdentifierInFile)
		
		;

	m.def("Scene_GetRootGameObjects", Scene_GetRootGameObjects);
	
	// Scene
	class_<Scene>(m, "Scene")
		.def("Clean", &Scene::Clean)
		.def("GetHandle", &Scene::GetHandle)
		//.def_static("GetRootGameObjects", [](const py::object&, int handle) { SceneManager::GetSceneByHandle(handle)->re})
	;

	class_<SceneManager>(m, "SceneManager")
		.def_static("CreateScene", &SceneManager::CreateScene, return_value_policy::reference)
		.def_static("GetActiveScene", &SceneManager::GetActiveScene, return_value_policy::reference)
		.def_static("SetActiveScene", &SceneManager::SetActiveScene, return_value_policy::reference);



#define DefineFunc(classname) \
	m.def("Create" #classname, []() { return new classname(); }, pybind11::return_value_policy::reference); \
	m.def(#classname "ClassID", []() ->int { return classname::ClassID; })

	//m.def("CreateGameObject", []() { return new GameObject(); }, return_value_policy::reference);
	//m.def("GameObjectClassID", []() ->int { return GameObject::ClassID; });

	//m.def("CreateScript", []() { return new Script(); }, return_value_policy::reference);

	DefineFunc(Prefab);
	class_<Prefab, Object>(m, "Prefab");

	// GameObject
	DefineFunc(GameObject);
	class_<GameObject, Object>(m, "GameObject")
		//.def(init<>())
		//.def(init<const std::string&>())
		//.def_property_readonly_static("ClassID", [](const py::object&) { return (int)GameObject::ClassID; })
		.def_property_readonly("transform", &GameObject::GetTransform, return_value_policy::reference)
		.def("AddComponent", &GameObject::AddComponent)
		.def("GetScene", &GameObject::GetScene, return_value_policy::reference)
		.def("IsActive", &GameObject::IsActive)
		.def("SetActive", &GameObject::SetActive)
		.def("IsActiveInHierarchy", &GameObject::IsActiveInHierarchy)
	// compile error in clang
//		.def("GetComponent", py::overload_cast<int>(&GameObject::GetComponent), return_value_policy::reference)
		.def("GetComponent_internal", (Component* (GameObject::*)(int))&GameObject::GetComponent, return_value_policy::reference)
//		.def("GetPrefabInternal", &GameObject::GetPrefabInternal, return_value_policy::reference)
	;

	//m.def("GameObject_GetComopnent", &GameObject_GetComopnent);
	m.def("GameObject_GetAllComponents", &GameObject_GetAllComponents);


	class_<Component, Object>(m, "Component")
		.def("GetGameObject", &Component::GetGameObject, return_value_policy::reference)
//		.def("GetPrefabInternal", &Component::GetPrefabInternal, return_value_policy::reference)
	;


	m.def("TransformClassID", []() ->int { return Transform::ClassID; });
	// Transform
	class_<Transform, Component>(m, "Transform")
		//.def_property_readonly_static("ClassID", [](py::object) { return (int)Transform::ClassID; })
		.def_property("localRotation", &Transform::GetLocalRotation, &Transform::SetLocalRotation)
		.def_property("localPosition", &Transform::GetLocalPosition, py::overload_cast<const Vector3&>(&Transform::SetLocalPosition))
		.def_property("localScale", &Transform::GetLocalScale, py::overload_cast<const Vector3&>(&Transform::SetLocalScale))
		.def_property("localEulerAngles", &Transform::GetLocalEulerAngles, py::overload_cast<const Vector3&>(&Transform::SetLocalEulerAngles))
		.def_property("rotation", &Transform::GetRotation, &Transform::SetRotation)
		.def_property("position", &Transform::GetPosition, py::overload_cast<const Vector3&>(&Transform::SetPosition))

		.def("SetEulerAngles", &Transform::SetEulerAngles)
		//.def("SetParent", &Transform::SetParent, "worldPositionStays"_a=true)
		.def("GetParent", &Transform::GetParent, return_value_policy::reference)
		.def("SetParent", &Transform::SetParent)
//		.def_property("parent", &Transform::GetParent, &Transform::SetParent, return_value_policy::reference)
		.def_property("rootOrder", &Transform::GetRootOrder, &Transform::SetRootOrder)
		.def("localToWorldMatrix", &Transform::GetLocalToWorldMatrix, return_value_policy::copy)
		.def("worldToLocalMatrix", &Transform::GetWorldToLocalMatrix)
		.def("RotateAround", &Transform::RotateAround)
		.def("GetSiblingIndex", &Transform::GetSiblingIndex)
		.def("SetSiblingIndex", &Transform::SetSiblingIndex)
		//.def_property_readonly("children", &Transform::GetChildren);
	;

	m.def("Transform_GetChildren", &Transform_GetChildren);

	DefineFunc(RectTransform);
	class_<RectTransform, Component>(m, "RectTransform")
		//.def(init<>())
		.def_readwrite("m_AnchorMin", &RectTransform::m_AnchorMin)
		.def_readwrite("m_AnchorMax", &RectTransform::m_AnchorMax)
		.def_readwrite("m_AnchoredPosition", &RectTransform::m_AnchoredPosition)
		.def_readwrite("m_SizeDelta", &RectTransform::m_SizeDelta)
		.def_readwrite("m_Pivot", &RectTransform::m_Pivot)
		;

	DefineFunc(Script);
	class_<Script, Component, PyScript>(m, "Script")
		.def(init<>())
		.def("GetPyObject", &Script::GetPyObject)
		.def("SetPyObject", &Script::SetPyObject)
	;

	class_<Mesh, Object>(m, "Mesh")
		.def_static("FromTextFile", &Mesh::FromTextFile, return_value_policy::take_ownership)
		;

	class_<Shader, Object>(m, "Shader")
//		.def_static("FromString", &Shader::FromString, return_value_policy::take_ownership)
		;

	//DefineFunc(Material);
	class_<Material, Object>(m, "Material")
		.def(init<>())
		.def_property("shader", &Material::GetShader, &Material::SetShader, return_value_policy::reference)
		//.def_property_readonly_static("defaultMaterial", &Material::GetDefaultMaterial, return_value_policy::reference)
		//.def_static("GetDefaultMaterial", &Material::GetDefaultMaterial, return_value_policy::reference)
		//.def_property_readonly_static("errorMaterial", &Material::GetErrorMaterial, return_value_policy::reference)
		;

	DefineFunc(MeshFilter);
	class_<MeshFilter, Component>(m, "MeshFilter")
		.def_property("mesh", &MeshFilter::GetMesh, &MeshFilter::SetMesh)
	;


	class_<Renderer, Component>(m, "Renderer")
			.def_property("enabled", &Renderer::GetEnabled, &Renderer::SetEnabled)
			.def_property("shadowCastingMode", &Renderer::GetCastShadows, &Renderer::SetCastShadows)
			.def_property("receiveShadows", &Renderer::GetReceiveShadows, &Renderer::SetReceiveShadows)
			.def_property("material", &Renderer::GetMaterial, &Renderer::SetMaterial)
					;

	DefineFunc(MeshRenderer);
	class_<MeshRenderer, Renderer>(m, "MeshRenderer")
	;
	
	
	m.def("SkinnedMeshRenderer__GetBones", &SkinnedMeshRenderer__GetBones);
	DefineFunc(SkinnedMeshRenderer);
	class_<SkinnedMeshRenderer, Renderer>(m, "SkinnedMeshRenderer")
	;

	class_<Graphics>(m, "Graphics")
//		.def_static("DrawMesh", &Graphics::DrawMesh)
		;
	
	class_<Gizmos>(m, "Gizmos")
	.def_property_static("color",
						 [](py::object) { return Gizmos::color(); },
						 [](py::object, Color c) { Gizmos::SetColor(c); })
	.def_static("DrawWireSphere", &Gizmos::DrawWireSphere)
	.def_static("DrawLine", &Gizmos::DrawLine)
	;

	DefineFunc(Camera);
	class_<Camera, Component>(m, "Camera")
		.def_property("farClipPlane", &Camera::GetFarClipPlane, &Camera::SetFarClipPlane)
		.def_property("nearClipPlane", &Camera::GetNearClipPlane, &Camera::SetNearClipPlane)
		.def_property("fieldOfView", &Camera::GetFieldOfView, &Camera::SetFieldOfView)
		.def_property("orthographic", &Camera::GetOrthographic, &Camera::SetOrthographic)
		.def_property("orthographicSize", &Camera::GetOrthographicSize, &Camera::SetOrthographicSize)
		.def("GetProjectionMatrix", &Camera::GetProjectionMatrix, return_value_policy::reference)
		.def("GetWorldToCameraMatrix", &Camera::GetWorldToCameraMatrix)
		;

	class_<Screen>(m, "Screen")
		.def_property_readonly_static("width", [](py::object) { return Screen::GetWidth(); })
		.def_property_readonly_static("height", [](py::object) { return Screen::GetHeight(); })
		.def_static("SetResolution", &Screen::SetResolution)
		;

	DefineFunc(Light);
	class_<Light, Component>(m, "Light")
	;

	class_<Collider, Component>(m, "Collider");

	DefineFunc(BoxCollider);
	class_<BoxCollider, Collider>(m, "BoxCollider")
		.def_property("center", &BoxCollider::GetCenter, &BoxCollider::SetCenter)
		.def_property("size", &BoxCollider::GetSize, &BoxCollider::SetSize)
	;

	DefineFunc(SphereCollider);
	class_<SphereCollider, Collider>(m, "SphereCollider")
		.def_property("center", &SphereCollider::GetCenter, &SphereCollider::SetCenter)
		.def_property("radius", &SphereCollider::GetRadius, &SphereCollider::SetRadius)
		;

	DefineFunc(Rigidbody);
	class_<Rigidbody, Component>(m, "Rigidbody")
		.def_property("mass", &Rigidbody::GetMass, &Rigidbody::SetMass)
		.def_property("drag", &Rigidbody::GetDrag, &Rigidbody::SetDrag)
		.def_property("angularDrag", &Rigidbody::GetAngularDrag, &Rigidbody::SetAngularDrag)
		.def_property("useGravity", &Rigidbody::GetUseGravity, &Rigidbody::SetUseGravity)
		.def_property("isKinematic", &Rigidbody::GetIsKinematic, &Rigidbody::SetIsKinematic)
	;
}









//#if FISHENGINE_PLATFORM_WINDOWS
//const char* code = R"(
//import sys, os
//sys.path.insert(0, r'D:\program\FishEngine-Experiment\Script')
//)";
//#else
//const char* code = R"(
//import sys, os
//sys.path.insert(0, r'/Users/yushroom/program/FishEngine-Experiment/Script')
//)";
//#endif

namespace FishEngine
{
	void ScriptSystem::Init()
	{
		py::initialize_interpreter();
		auto main = py::module::import("__main__");
		auto globals = main.attr("__dict__");
		py::module::import("FishEngineInternal");
//		py::exec(code, globals);
		
#if FISHENGINE_PLATFORM_WINDOWS
		const char* script_root_dir = R"(D:\program\FishEngine-Experiment\Script)";
#else
		const char* script_root_dir = "/Users/yushroom/program/FishEngine-Experiment/Script";
#endif
		ScriptSystem::AddScriptDir(script_root_dir);
	}
	
	void ScriptSystem::Start()
	{
		auto app = py::module::import("app");
		app.attr("Start")();
	}

	void ScriptSystem::Update()
	{
		auto app = py::module::import("app");
		app.attr("Update")();
	}
	
	
	void ScriptSystem::Clean()
	{
		// note this {}
		{
			auto app = py::module::import("app");
			app.attr("Clean")();
		}
		py::finalize_interpreter();
	}
	
	void ScriptSystem::Reload()
	{
		Clean();
		auto app = py::module::import("app");
		app.attr("Reload")();
		Init();
	}
	
	void ScriptSystem::AddScriptDir(const std::string& path)
	{
		auto code = Format("import sys, os\nsys.path.insert(0, r'{}')", path);
		auto main = py::module::import("__main__");
		auto globals = main.attr("__dict__");
		py::exec(code, globals);
	}
}

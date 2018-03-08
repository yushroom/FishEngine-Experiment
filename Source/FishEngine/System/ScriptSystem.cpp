#include <FishEngine/System/ScriptSystem.hpp>
#include <FishEditor/FishEditor.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
namespace py = pybind11;



#include <FishEngine/FishEngine.hpp>
#include <FishEngine/Math/Vector2.hpp>
#include <FishEngine/Math/Vector3.hpp>
#include <FishEngine/Math/Quaternion.hpp>
#include <FishEngine/Math/Matrix4x4.hpp>
#include <FishEngine/Object.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/RectTransform.hpp>
#include <FishEngine/Script.hpp>

#include <FishEngine/Render/Mesh.hpp>
#include <FishEngine/Render/Shader.hpp>
#include <FishEngine/Render/Material.hpp>

#include <FishEngine/Component/MeshFilter.hpp>
#include <FishEngine/Component/MeshRenderer.hpp>
#include <FishEngine/Component/Camera.hpp>
#include <FishEngine/Component/Light.hpp>

#include <FishEngine/Render/Graphics.hpp>
#include <FishEngine/Screen.hpp>
#include <FishEngine/Scene.hpp>

#include <pybind11/embed.h>
#include <pybind11/operators.h>

using namespace FishEngine;
namespace py = pybind11;


py::handle FindObjectOfType(int classID)
{
	auto& objs = Object::FindObjectsOfType(classID);
	if (objs.size() == 0)
		return py::none{};
	return (*objs.begin())->GetPyObject();
}

py::object FindObjectsOfType(int classID)
{
	auto& objs = Object::FindObjectsOfType(classID);
	py::list ret;
	for (auto o : objs)
	{
		auto self = o->GetPyObject();
		assert(!self.is(py::none()));
		ret.append(self);
	}
	return ret;
}


#define DefObject(classname) \
	m.def("Create"##classname, [](){ return new classname();}, return_value_policy::reference); \
	m.def(#classname"ClassID", []() { return classname::ClassID;  })

PYBIND11_EMBEDDED_MODULE(FishEngineInternal, m)
{
	using namespace py;

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

	class_<Matrix4x4>(m, "Matrix4x4")
		.def("__str__", &Matrix4x4::ToString)
		.def(self*Matrix4x4())
		.def_static("Perspective", &Matrix4x4::Perspective)
		.def_static("LookAt", &Matrix4x4::LookAt)
		.def("MultiplyPoint", &Matrix4x4::MultiplyPoint)
		;

	// Object
	class_<Object>(m, "Object")
		.def_readwrite("name", &Object::name)
		.def_readonly("instanceID", &Object::instanceID)
		.def("SetObject", &Object::SetObject)
		//		.def("Destroy", &Object::Destroy).staticmethod("Destroy")
		;

	class_<Scene>(m, "Scene");

	class_<SceneManager>(m, "SceneManager");


	m.def("CreateGameObject", []() { return new GameObject(); }, return_value_policy::reference);
	m.def("GameObjectClassID", []() ->int { return GameObject::ClassID; });

	m.def("CreateScript", []() { return new Script(); }, return_value_policy::reference);


	// GameObject
	class_<GameObject, Object>(m, "GameObject")
		.def(init<>())
		.def(init<const std::string&>())
		.def_property_readonly_static("ClassID", [](py::object) { return (int)GameObject::ClassID; })
		.def_readonly("transform", &GameObject::m_transform)
		.def("AddComponent", &GameObject::AddComponent)
		;

	class_<Component, Object>(m, "Component")
		.def_readwrite("gameObject", &Component::m_gameObject);
	;

	//http://www.boost.org/doc/libs/1_66_0/libs/python/doc/html/reference/function_invocation_and_creation/models_of_callpolicies.html

	//m.def("CreateTransform", []() { return new Transform(); }, return_value_policy::reference);
	m.def("TransformClassID", []() { return Transform::ClassID; });

	// Transform
	class_<Transform, Component>(m, "Transform")
		.def("localPosition", &Transform::localPosition)
		.def("localRotation", &Transform::localRotation)
		.def("localEulerAngles", &Transform::localEulerAngles)
		.def("localScale", &Transform::localScale)
		.def("position", &Transform::position)
		.def("rotation", &Transform::rotation)
		.def("eulerAngles", &Transform::eulerAngles)
		.def("SetLocalPosition", &Transform::setLocalPosition)
		.def("SetLocalRotation", &Transform::setLocalRotation)
		.def("SetLocalEulerAngles", &Transform::setLocalEulerAngles)
		.def("SetLocalScale", &Transform::setLocalScale)
		.def("SetPosition", &Transform::setPosition)
		.def("SetRotation", &Transform::setRotation)
		.def("SetEulerAngles", &Transform::setEulerAngles)
		.def("GetParent", &Transform::GetParent, return_value_policy::reference)
		.def("SetParent", &Transform::SetParent)
		.def_property("m_RootOrder", &Transform::GetRootOrder, &Transform::SetRootOrder)
		.def("localToWorldMatrix", &Transform::localToWorldMatrix, return_value_policy::copy)
		.def("worldToLocalMatrix", &Transform::worldToLocalMatrix)
		.def("RotateAround", &Transform::RotateAround)
		;

	class_<RectTransform, Component>(m, "RectTransform")
		.def(init<>())
		.def_readwrite("m_AnchorMin", &RectTransform::m_AnchorMin)
		.def_readwrite("m_AnchorMax", &RectTransform::m_AnchorMax)
		.def_readwrite("m_AnchoredPosition", &RectTransform::m_AnchoredPosition)
		.def_readwrite("m_SizeDelta", &RectTransform::m_SizeDelta)
		.def_readwrite("m_Pivot", &RectTransform::m_Pivot)
		;

#if 1
	class_<Script, Component>(m, "Script")
		.def(init<>())
	;
#else
	// Script
	class_<ScriptWrap, bases<Component>, Script*, boost::noncopyable>("Script")
		//        .def("SetObject", &Script::SetObject)
		.def("Start", &Script::Start, &ScriptWrap::default_Start)
		.def("Update", &Script::Update, &ScriptWrap::default_Update)
		;
#endif

	class_<Mesh, Object>(m, "Mesh")
		.def_static("FromTextFile", &Mesh::FromTextFile, return_value_policy::take_ownership)
		;

	class_<Shader, Object>(m, "Shader")
		.def_static("FromString", &Shader::FromString, return_value_policy::take_ownership)
		;

	class_<Material, Object>(m, "Material")
		.def(init<>())
		.def_property("shader", &Material::GetShader, &Material::SetShader, return_value_policy::reference)
		.def_static("GetErrorMaterial", &Material::GetErrorMaterial, return_value_policy::reference)
		;

	class_<MeshFilter, Component>(m, "MeshFilter")
		.def_property_readonly_static("ClassID", [](py::object) { return (int)MeshFilter::ClassID; })
		.def(init<>())
		.def_readwrite("mesh", &MeshFilter::m_mesh)
		;

	class_<MeshRenderer, Component>(m, "MeshRenderer")
		.def_property_readonly_static("ClassID", [](py::object) { return (int)MeshRenderer::ClassID; })
		.def(init<>())
		.def_readwrite("material", &MeshRenderer::m_material)
		;

	class_<Graphics>(m, "Graphics")
		.def_static("DrawMesh", &Graphics::DrawMesh)
		;

	class_<Camera, Component>(m, "Camera")
		.def_property_readonly_static("ClassID", [](py::object){ return (int)Camera::ClassID; })
		.def(init<>())
		.def_property("fieldOfView", &Camera::fieldOfView, &Camera::setFieldOfView)
		.def_property_readonly("aspect", &Camera::aspect)
		.def_property("orthographic", &Camera::GetOrthographic, &Camera::SetOrthographic)
		.def_property("nearClipPlane", &Camera::nearClipPlane, &Camera::setNearClipPlane)
		.def_property("farClipPlane", &Camera::farClipPlane, &Camera::setFarClipPlane)
		.def_property("orthographicSize", &Camera::orthographicSize, &Camera::setOrthographicSize)
		.def("projectionMatrix", &Camera::projectionMatrix, return_value_policy::reference)
		.def("worldToCameraMatrix", &Camera::worldToCameraMatrix)
		;

	class_<Screen>(m, "Screen")
		.def_property_readonly_static("width", [](py::object) { return Screen::width(); })
		.def_property_readonly_static("height", [](py::object) { return Screen::height(); })
		.def_static("SetResolution", &Screen::SetResolution)
		;

	class_<Light, Component>(m, "Light")
		.def(init<>())
		.def_property_readonly_static("ClassID", [](py::object) { return (int)Light::ClassID; })
	;
}









#if FISHENGINE_PLATFORM_WINDOWS
const char* code = R"(
import sys, os
sys.path.insert(0, r'D:\program\FishEngine-Experiment\Script')
)";
#else
const char* code = R"(
import sys, os
sys.path.insert(0, r'/Users/yushroom/program/FishEngine-Experiment/Script')
)";
#endif

namespace FishEngine
{
	void ScriptSystem::Init()
	{
		py::initialize_interpreter();
		auto main = py::module::import("__main__");
		auto globals = main.attr("__dict__");
		py::module::import("FishEngineInternal");
		py::exec(code, globals);
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
}

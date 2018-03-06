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

#include <boost/python.hpp>
//#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using namespace FishEngine;
namespace py = boost::python;

// https://stackoverflow.com/questions/6157409/stdvector-to-boostpythonlist/6174586
// Converts a C++ vector to a python list
template <class T>
py::list toPythonList(const std::vector<T>& v)
{
	typename std::vector<T>::iterator iter;
	py::list l;
	for (auto& t : v)
	{
		l.append(t);
	}
	return l;
}


//static std::unordered_map<int, PyObject*> s_pyobjects;
//
//void Register(int instanceID, PyObject* obj)
//{
//	s_pyobjects[instanceID] = obj;
//}
//
//void Unregister(int instanceID)
//{
//	s_pyobjects.erase(instanceID);
//}

PyObject* FindObjectOfType(int classID)
{
	auto& objs = Object::FindObjectsOfType(classID);
	if (objs.size() == 0)
		return nullptr;
	return (*objs.begin())->GetPyObject();
}

PyObject* FindObjectsOfType(int classID)
{
	auto& objs = Object::FindObjectsOfType(classID);
	PyObject* ret = PyList_New(0);
	for (auto o : objs)
	{
		auto self = o->GetPyObject();
		assert(self != nullptr);
		PyList_Append(ret, self);
	}
//	return py::object(py::handle<>(ret));
	return ret;
	
//	PyObject* ret = PyList_New(0);
//	for (auto o : objs)
//	{
////		ret.append(boost::python::handle<>(s_pyobjects[o->instanceID]));
////		ret.append(s_pyobjects[o->instanceID]);
//		if (s_pyobjects.find(o->instanceID) == s_pyobjects.end())
//		{
//			abort();
//		}
//		PyList_Append(ret, s_pyobjects[o->instanceID]);
//	}
//	return py::object(py::handle<>(ret));
}

//typedef std::vector<int> int_vector;

#define DefClassID(classname) \
static const int ClassID_##classname() \
{ \
	return classname::ClassID; \
}

static const int ClassID_GameObject()
{
	return GameObject::ClassID;
}

DefClassID(Transform)
DefClassID(Camera)
DefClassID(Light)
DefClassID(MeshFilter)
DefClassID(MeshRenderer)

BOOST_PYTHON_MODULE(FishEngineInternal)
{
    using namespace boost::python;

//    def("Run", Run);
	
//	class_<int_vector>("int_vector")
//		.def(vector_indexing_suite<int_vector>());
	
//	def("Register", &Register);
//	def("Unregister", &Unregister);
	def("FindObjectOfType", &FindObjectOfType);
	def("FindObjectsOfType", &FindObjectsOfType);
	
	// Vector2
	class_<Vector2>("Vector2")
		.def(init<>())
		.def(init<float, float>())
		.def_readwrite("x", &Vector2::x)
		.def_readwrite("y", &Vector2::y)
		.def("Set", &Vector2::Set)
		.def(self+Vector2())
		.def(self-Vector2())
		.def(self*float())
		.def(float()*self)
	;

    // Vector3
    class_<Vector3>("Vector3")
		.def(init<>())
        .def(init<float, float, float>())
        .def_readwrite("x", &Vector3::x)
        .def_readwrite("y", &Vector3::y)
        .def_readwrite("z", &Vector3::z)
        .def("Set", &Vector3::Set)
		.def(self+Vector3())
		.def(self-Vector3())
		.def(self*Vector3())
		.def(self*float())
		.def(float()*self)
		.def("Dot", &Vector3::Dot).staticmethod("Dot")
    ;


    // Quaternion
    class_<Quaternion>("Quaternion")
		.def(init<>())
		.def(init<float, float, float, float>())
        .def_readwrite("x", &Quaternion::x)
        .def_readwrite("y", &Quaternion::y)
        .def_readwrite("z", &Quaternion::z)
        .def_readwrite("w", &Quaternion::w)
        .def("Set", &Quaternion::Set)
		.def("LookRotation", &Quaternion::LookRotation).staticmethod("LookRotation")
//		.add_property("eulerAngles", &Quaternion::GetEulerAngles, &Quaternion::SetEulerAngles)
    ;
	
	class_<Matrix4x4>("Matrix4x4")
		.def("__str__", &Matrix4x4::ToString)
		.def(self*Matrix4x4())
		.def("Perspective", &Matrix4x4::Perspective).staticmethod("Perspective")
		.def("LookAt", &Matrix4x4::LookAt).staticmethod("LookAt")
		.def("MultiplyPoint", &Matrix4x4::MultiplyPoint)
	;

    // Object
	class_<Object, Object*, boost::noncopyable>("Object", no_init)
        .def_readwrite("name", &Object::name)
        .def_readonly("instanceID", &Object::instanceID)
		.def("SetObject", &Object::SetObject)
//		.def("Destroy", &Object::Destroy).staticmethod("Destroy")
    ;
	
	class_<Scene, Scene*, boost::noncopyable>("Scene");
	
	class_<SceneManager>("SceneManager", no_init);
	

    // GameObject
    class_<GameObject, bases<Object>, GameObject*, boost::noncopyable>("GameObject", no_init)
	.add_static_property("ClassID", ClassID_GameObject)
//		.def(init<>())
//        .def(init<const std::string&>())
		.def("Create", GameObject::Create, return_value_policy<manage_new_object>())
//		.def("CreateWithTransform", &GameObject::CreateWithTransform, return_value_policy<manage_new_object>())
//		.def("CreateWithRectTransform", &GameObject::CreateWithRectTransform, return_value_policy<manage_new_object>())
//		.def("AddRectTransform", &GameObject::AddRectTransform)
        .def_readonly("transform", &GameObject::m_transform)
        .def("AddComponent", &GameObject::AddComponent)
    ;

	class_<Component, bases<Object>, Component*, boost::noncopyable>("Component", no_init)
		.def_readwrite("gameObject", &Component::m_gameObject);
	;

	//http://www.boost.org/doc/libs/1_66_0/libs/python/doc/html/reference/function_invocation_and_creation/models_of_callpolicies.html
	
    // Transform
    class_<Transform, bases<Component>, Transform*, boost::noncopyable>("Transform", no_init)
		.add_static_property("ClassID", ClassID_Transform)
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
		.def("GetParent", &Transform::GetParent, return_internal_reference<>())
		.def("SetParent", &Transform::SetParent)
		.add_property("m_RootOrder", &Transform::GetRootOrder, &Transform::SetRootOrder)
		.def("localToWorldMatrix", &Transform::localToWorldMatrix, return_value_policy<copy_const_reference>())
		.def("worldToLocalMatrix", &Transform::worldToLocalMatrix)
		.def("RotateAround", &Transform::RotateAround)
    ;
	
	class_<RectTransform, bases<Component>, RectTransform*, boost::noncopyable>("RectTransform")
	.def_readwrite("m_AnchorMin", &RectTransform::m_AnchorMin)
	.def_readwrite("m_AnchorMax", &RectTransform::m_AnchorMax)
	.def_readwrite("m_AnchoredPosition", &RectTransform::m_AnchoredPosition)
	.def_readwrite("m_SizeDelta", &RectTransform::m_SizeDelta)
	.def_readwrite("m_Pivot", &RectTransform::m_Pivot)
	;

#if 1
	class_<Script, bases<Component>, Script*, boost::noncopyable>("Script")
	;
#else
    // Script
	class_<ScriptWrap, bases<Component>, Script*, boost::noncopyable>("Script")
//        .def("SetObject", &Script::SetObject)
		.def("Start", &Script::Start, &ScriptWrap::default_Start)
		.def("Update", &Script::Update, &ScriptWrap::default_Update)
    ;
#endif
	
	class_<Mesh, bases<Object>, Mesh*, boost::noncopyable>("Mesh", no_init)
		.def("FromTextFile", &Mesh::FromTextFile, return_value_policy<manage_new_object>())
		.staticmethod("FromTextFile")
	;
	
	class_<Shader, Shader*, boost::noncopyable>("Shader", no_init)
		.def("FromString", &Shader::FromString, return_value_policy<manage_new_object>())
		.staticmethod("FromString")
	;
	
	class_<Material, bases<Object>, Material*, boost::noncopyable>("Material", no_init)
		.add_property("shader", make_function(&Material::GetShader, return_internal_reference<>()), &Material::SetShader)
		.def("Create", &Material::Create, return_value_policy<manage_new_object>()).staticmethod("Create")
		.def("GetErrorMaterial", &Material::GetErrorMaterial, return_internal_reference<>()).staticmethod("GetErrorMaterial")
	;

	class_<MeshFilter, bases<Component>, MeshFilter*, boost::noncopyable>("MeshFilter")
		.add_static_property("ClassID", ClassID_MeshFilter)
		.def_readwrite("mesh", &MeshFilter::m_mesh)
	;
	
	class_<MeshRenderer, bases<Component>, MeshRenderer*, boost::noncopyable>("MeshRenderer")
		.add_static_property("ClassID", ClassID_MeshRenderer)
		.def_readwrite("material", &MeshRenderer::m_material)
	;
	
	class_<Graphics>("Graphics", no_init)
		.def("DrawMesh", &Graphics::DrawMesh)
		.staticmethod("DrawMesh")
	;
	
	class_<Camera, bases<Component>, Camera*, boost::noncopyable>("Camera")
		.add_static_property("ClassID", ClassID_Camera)
		.add_property("fieldOfView", &Camera::fieldOfView, &Camera::setFieldOfView)
		.add_property("aspect", &Camera::aspect)
		.add_property("orthographic", &Camera::GetOrthographic, &Camera::SetOrthographic)
		.add_property("nearClipPlane", &Camera::nearClipPlane, &Camera::setNearClipPlane)
		.add_property("farClipPlane", &Camera::farClipPlane, &Camera::setFarClipPlane)
		.add_property("orthographicSize", &Camera::orthographicSize, &Camera::setOrthographicSize)
		.def("projectionMatrix", &Camera::projectionMatrix, return_value_policy<copy_const_reference>())
		.def("worldToCameraMatrix", &Camera::worldToCameraMatrix)
	;
	
	class_<Screen>("Screen", no_init)
		.add_static_property("width", &Screen::width)
		.add_static_property("height", &Screen::height)
		.def("SetResolution", &Screen::SetResolution).staticmethod("SetResolution")
	;
	
	class_<Light, bases<Component>, Light*, boost::noncopyable>("Light")
		.add_static_property("ClassID", ClassID_Light)
	;
}

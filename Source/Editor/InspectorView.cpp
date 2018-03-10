#include "InspectorView.hpp"

#include <FishEditor/Selection.hpp>

using namespace FishEngine;
using namespace FishEditor;

#include <FishEngine/Render/Mesh.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/RectTransform.hpp>
#include <FishEngine/Component/Camera.hpp>
#include <FishEngine/Component/Light.hpp>
#include <FishEngine/Component/MeshFilter.hpp>
#include <FishEngine/Component/MeshRenderer.hpp>
#include <FishEngine/Script.hpp>

void DrawTransform(Transform* t)
{
	auto p = t->GetLocalPosition();
	auto r = t->GetLocalEulerAngles();
	auto s = t->GetLocalScale();

	//FishGUI::Group("Transform");
	FishGUI::Float3("Position", p.x, p.y, p.z);
	FishGUI::Float3("Rotation", r.x, r.y, r.z);
	FishGUI::Float3("Scale", s.x, s.y, s.z);
	//FishGUI::EndGroup();
}

void DrawCamera(Camera* c)
{
	float znear = c->GetNearClipPlane();
	float zfar = c->GetFarClipPlane();
	float fov = c->GetFieldOfView();
	if (c->GetOrthographic())
	{
		std::string o = "Orthographic";
		FishGUI::Combox("Projection", o);
		float size = c->GetOrthographicSize();
		FishGUI::Float("Size", size);
	}
	else
	{
		std::string o = "Perspective";
		FishGUI::Combox("Projection", o);
		FishGUI::Float("Field of View", fov);
	}
	FishGUI::Float("Z Near", znear);
	FishGUI::Float("Z Far", zfar);
}

void DrawLight(Light* l)
{
}

void DrawMeshRenderer(MeshRenderer* mr)
{

}

void DrawMeshFilter(MeshFilter* mf)
{
	auto meshName = mf->GetMesh()->GetName();
	FishGUI::InputText("Mesh", meshName);
}

/// @brief callable() support for Boost.Python objects.
bool IsCallable(pybind11::handle object)
{
	return 1 == PyCallable_Check(object.ptr());
}

std::string TypeName(pybind11::handle object)
{
	//return object.attr("__class__").attr("__name__").cast<std::string>();
	//return (std::string) pybind11::str(object.get_type());
	return object.ptr()->ob_type->tp_name;
}

bool StartsWith(const std::string& a, const std::string& b)
{
	if (a.size() < b.size())
		return false;
	for (int i = 0; i < b.size(); ++i)
		if (a[i] != b[i])
			return false;
	return true;
}

void DrawScript(Script* s)
{
	auto handle = s->GetPyObject();
	std::string scriptName = handle.ptr()->ob_type->tp_name;
	//std::string scriptName = (std::string) pybind11::str(handle.get_type());
	FishGUI::Group(scriptName + "(Script)");
	FishGUI::Text("Script", scriptName);

	auto instanceID_str = std::to_string(s->GetInstanceID());
	FishGUI::Text("Instance ID", instanceID_str);

	auto attributes = handle.attr("GetVisiableAttributes")();		// list of str
	for (const auto& a : attributes)
	{
	//auto dir = pybind11::handle(PyObject_Dir(handle.ptr()));
	//for (auto a : dir)
	//{
		auto name = a.cast<std::string>();
		auto attribute = handle.attr(name.c_str());
		//if (!StartsWith(name, "__") && !IsCallable(attribute))
		{
			//auto typeName = attribute.attr("__class__").attr("__name__").cast<std::string>();
			std::string typeName = TypeName(attribute);
			std::string value = "";

			if (typeName == "float")
			{
				value = std::to_string( attribute.cast<float>() );
				FishGUI::Text(name.c_str(), value);
			}
			else if (typeName == "int")
			{
				value = std::to_string(attribute.cast<int>());
				FishGUI::Text(name.c_str(), value);
			}
			else
			{
				FishGUI::Text(name.c_str(), typeName);
			}
			
		}
	}
	//pybind11::print(pybind11::handle(dir));

	FishGUI::EndGroup();
}


void Dispatch(Component* c)
{
	if (c->GetClassID() == Transform::ClassID)
	{
		FishGUI::Group("Transform");
		DrawTransform((Transform*)c);
		FishGUI::EndGroup();
	}
	else if (c->GetClassID() == Camera::ClassID)
	{
		FishGUI::Group("Camera");
		DrawCamera((Camera*)c);
		FishGUI::EndGroup();
	}
	else if (c->GetClassID() == Light::ClassID)
	{
		FishGUI::Group("Light");
		DrawLight((Light*)c);
		FishGUI::EndGroup();
	}
	else if (c->GetClassID() == MeshFilter::ClassID)
	{
		FishGUI::Group("MeshFilter");
		DrawMeshFilter((MeshFilter*)c);
		FishGUI::EndGroup();
	}
	else if (c->GetClassID() == MeshRenderer::ClassID)
	{
		FishGUI::Group("MeshRenderer");
		DrawMeshRenderer((MeshRenderer*)c);
		FishGUI::EndGroup();
	}
	else if (c->GetClassID() == Script::ClassID)
	{
		DrawScript((Script*)c);
	}
}

void InspectorView::DrawImpl()
{
	auto t = Selection::GetActiveTransform();
	if (t == nullptr)
		return;

	for (auto c : t->GetGameObject()->GetAllComponents())
	{
		Dispatch(c);
	}
}

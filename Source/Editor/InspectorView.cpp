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
#include <FishEngine/Render/Material.hpp>
#include <FishEngine/Component/BoxCollider.hpp>
#include <FishEngine/Component/SphereCollider.hpp>
#include <FishEngine/Component/Rigidbody.hpp>

#include <FishEditor/AssetImporter.hpp>
#include <FishEditor/FBXImporter.hpp>


template<class T, class Getter>
void Bool(const char* label, T* o, Getter getter)
{
	bool v = (o->*getter)();
	FishGUI::CheckBox(label, v);
}


template<class T, class Getter>
void String(const char* label, T* o, Getter getter)
{
	std::string v = (o->*getter)();
	FishGUI::InputText(label, v);
}

template<class T, class Getter>
void Float(const char* label, T* o, Getter getter)
{
	float v = (o->*getter)();
	FishGUI::Float(label, v);
}

template<class T, class Getter, class Setter>
void Float(const char* label, T* o, Getter getter, Setter setter)
{
	float v = (o->*getter)();
	if (FishGUI::Float(label, v))
	{
		(o->*setter)(v);
	}
}

template<class T, class Getter>
void Float3(const char* label, T* o, Getter getter)
{
	auto v = (o->*getter)();
	FishGUI::Float3(label, v.x, v.y, v.z);
}

//template<class T, class Getter, class Setter>
//void Float3(const char* label, T* o, Getter getter, Setter setter)
//{
//	auto v = (o->*getter)();
//	if (FishGUI::Float3(label, v.x, v.y, v.z))
//	{
//		(o->*setter)(v);
//	}
//}
template<class T, class Getter>
void Float3(const char* label, T* o, Getter getter, void(T::*setter)(const Vector3&))
{
	auto v = (o->*getter)();
	if (FishGUI::Float3(label, v.x, v.y, v.z))
	{
		(o->*setter)(v);
	}
}

template<class T, class Getter>
void Int(const char* label, T* o, Getter getter)
{
	auto int_str = std::to_string((o->*getter)());
	FishGUI::Text(label, int_str);
}


void DrawObject(Object* o)
{
	Int("Instance ID", o, &Object::GetInstanceID);
	Int("Local Identifier In File", o, &Object::GetLocalIdentifierInFile);
}

void DrawTransform(Transform* value)
{
	DrawObject(value);
	Float3("Position", value, &Transform::GetLocalPosition, &Transform::SetLocalPosition);
	Float3("Rotation", value, &Transform::GetLocalEulerAngles, &Transform::SetLocalEulerAngles);
	Float3("Scale", value, &Transform::GetLocalScale, &Transform::SetLocalScale);
	//auto m = value->GetLocalToWorldMatrix();
	//for (int row = 0; row < 4; ++ row)
	//{
	//	for (int col = 0;  col < 4; ++ col)
	//	{
	//		std::string label = "E" + std::to_string(row) + std::to_string(col);
	//		FishGUI::Float(label, m[col][row]);
	//	}
	//}
}

void DrawCamera(Camera* c)
{
	DrawObject(c);
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
	FishGUI::Float("Near Clip Plane", znear);
	FishGUI::Float("Far Clip Plane", zfar);
}

void DrawLight(Light* l)
{
	DrawObject(l);
}

void DrawMeshRenderer(MeshRenderer* mr)
{
	DrawObject(mr);
	Material* mat = mr->GetMaterial();
	std::string name = (mat == nullptr ? "None(Material)" : mat->GetName());
	FishGUI::InputText("Material", name);
}

void DrawMeshFilter(MeshFilter* mf)
{
	DrawObject(mf);
	auto mesh = mf->GetMesh();
	std::string meshName = (mesh == nullptr ? "None(Mesh)" : mesh->GetName());
	FishGUI::InputText("Mesh", meshName);
}

void DrawBoxCollider(BoxCollider* value)
{
	DrawObject(value);
	Float3("Center", value, &BoxCollider::GetCenter, &BoxCollider::SetCenter);
	Float3("Size", value, &BoxCollider::GetSize, &BoxCollider::SetSize);
}

void DrawSphereCollider(SphereCollider* value)
{
	DrawObject(value);
	Float3("Center", value, &SphereCollider::GetCenter, &SphereCollider::SetCenter);
	Float("Radius", value, &SphereCollider::GetRadius, &SphereCollider::SetRadius);
}


void DrawRigidbody(Rigidbody* r)
{
	DrawObject(r);
	Float("Mass", r, &Rigidbody::GetMass, &Rigidbody::SetMass);
	Float("Drag", r, &Rigidbody::GetDrag, &Rigidbody::SetDrag);
	Float("Angular Drag", r, &Rigidbody::GetAngularDrag, &Rigidbody::SetAngularDrag);
	Float("Use Gravity", r, &Rigidbody::GetUseGravity, &Rigidbody::SetUseGravity);
	Float("Is Kinematic", r, &Rigidbody::GetIsKinematic, &Rigidbody::SetIsKinematic);
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
	
	DrawObject(s);

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
			else if (typeName == "FishEngineInternal.Vector3")
			{
				auto vec = attribute.cast<FishEngine::Vector3>();
				FishGUI::Float3(name, vec.x, vec.y, vec.z);
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
	else if (c->GetClassID() == BoxCollider::ClassID)
	{
		FishGUI::Group("BoxCollider");
		DrawBoxCollider((BoxCollider*)c);
		FishGUI::EndGroup();
	}
	else if (c->GetClassID() == SphereCollider::ClassID)
	{
		FishGUI::Group("SphereCollider");
		DrawSphereCollider((SphereCollider*)c);
		FishGUI::EndGroup();
	}
	else if (c->GetClassID() == Rigidbody::ClassID)
	{
		FishGUI::Group("Rigidbody");
		DrawRigidbody((Rigidbody*)c);
		FishGUI::EndGroup();
	}
	else if (c->GetClassID() == Script::ClassID)
	{
		DrawScript((Script*)c);
	}
}



void Dispatch2(AssetImporter* i)
{
	String("GUID", i, &AssetImporter::GetGUID);
	//Int("GUID", i, &AssetImporter::GetGUID);
	if (i->GetClassID() == FishEditor::FBXImporter::ClassID)
	{
		FBXImporter* o = (FBXImporter*)i;
		Float("Scale Factor", o, &ModelImporter::GetGlobalScale);
		Bool("Use File Scale", o, &ModelImporter::GetUseFileScale);
		Float("File Scale", o, &ModelImporter::GetFileScale);
	}
}

void InspectorView::DrawImpl()
{
	auto t = Selection::GetActiveTransform();
	if (t != nullptr)
	{
		for (auto c : t->GetGameObject()->GetAllComponents())
		{
			Dispatch(c);
		}
		FishGUI::Button("Add Component");
		return;
	}

	auto o = Selection::GetActiveObject();
	if (o != nullptr)
	{
		if (o->Is<AssetImporter>())
		{
			Dispatch2((AssetImporter*)o);
		}
	}
}

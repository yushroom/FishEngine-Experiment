#include "InspectorWindow.hpp"

#include <FishEditor/Selection.hpp>

using namespace FishEngine;
using namespace FishEditor;


#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/RectTransform.hpp>
#include <FishEngine/Component/Camera.hpp>

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
}

void InspectorWindow::DrawImpl()
{
	auto t = Selection::GetActiveTransform();
	if (t == nullptr)
		return;

	for (auto c : t->GetGameObject()->GetAllComponents())
	{
		Dispatch(c);
	}
}
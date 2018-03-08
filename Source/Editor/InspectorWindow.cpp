#include "InspectorWindow.hpp"

#include <FishEditor/Selection.hpp>

using namespace FishEngine;
using namespace FishEditor;

void InspectorWindow::DrawImpl()
{
	auto t = Selection::GetActiveTransform();
	if (t == nullptr)
		return;

	auto p = t->GetLocalPosition();
	auto r = t->GetLocalEulerAngles();
	auto s = t->GetLocalScale();

	FishGUI::Group("Transform");
	FishGUI::Float3("Position", p.x, p.y, p.z);
	FishGUI::Float3("Rotation", r.x, r.y, r.z);
	FishGUI::Float3("Scale", s.x, s.y, s.z);
	FishGUI::EndGroup();
}
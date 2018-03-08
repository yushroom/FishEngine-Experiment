#pragma once
#include <FishGUI/Widget.hpp>
#include <FishGUI/FishGUI.hpp>


class InspectorWindow : public FishGUI::IMWidget
{
public:

	InspectorWindow(const char* name) : IMWidget(name)
	{

	}

	virtual void DrawImpl() override;
};
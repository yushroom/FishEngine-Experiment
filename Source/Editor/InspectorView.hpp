#pragma once
#include <FishGUI/Widget.hpp>
#include <FishGUI/FishGUI.hpp>


class InspectorView : public FishGUI::IMWidget
{
public:

	InspectorView(const char* name) : IMWidget(name)
	{

	}

	virtual void DrawImpl() override;
};

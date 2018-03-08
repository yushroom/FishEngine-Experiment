#include <FishGUI/FishGUI.hpp>
#include <FishGUI/Window.hpp>
#include <UnityLayout.hpp>
#include <UnityToolBar.hpp>

#include "SceneViewApp.hpp"
#include "HierarchyWidget.hpp"
#include "GLWidget.hpp"

#include <FishEditor/FishEditorInternal.hpp>

#include <pybind11/pybind11.h>
#include <iostream>

class IMWidget2 : public FishGUI::IMWidget
{
public:
	IMWidget2(const char* name) : IMWidget(name)
	{
	}

	virtual void DrawImpl() override
	{
		if (m_renderFunction != nullptr)
		{
			m_renderFunction();
		}
	}

	typedef std::function<void(void)> RenderFunction;

	void SetRenderFunction(RenderFunction func)
	{
		m_renderFunction = func;
	}

protected:
	RenderFunction	m_renderFunction;
};

int main()
try
{
	FishGUI::Init();
	auto win = FishGUI::NewWindow("Fish GUI", 950, 600);
	auto mainLayout = new UnityLayout();
	win->SetLayout(mainLayout);

	auto toolBar = new UnityToolBar();
	mainLayout->SetToolBar(toolBar);
	auto statusBar = new FishGUI::StatusBar();
	mainLayout->SetStatusBar(statusBar);

	auto right = new FishGUI::TabWidget("right");
	right->SetWidth(270);
	right->SetMinSize(250, 150);
	auto inspector = new IMWidget2("Inspector");
	right->AddChild(inspector);

	auto bottom = new FishGUI::TabWidget("bottom");
	bottom->SetHeight(180);
	bottom->SetMinSize(150, 150);
	auto project = new FishGUI::Widget("Project");
	auto console = new IMWidget2("Console");
	bottom->AddChild(project);
	bottom->AddChild(console);

	auto left = new FishGUI::TabWidget("Left");
	left->SetWidth(200);
	left->SetMinSize(200, 150);
	auto hierarchy = new HierarchyWidget("Hierarchy");
	left->AddChild(hierarchy);

	auto center = new FishGUI::TabWidget("Center");
	center->SetWidth(500);
	center->SetMinSize(150, 150);
	auto scene = new GLWidget("Scene");
	auto game = new IMWidget2("Game");
	auto assetStore = new IMWidget2("Asset Store");
	center->AddChild(scene);
	center->AddChild(game);
	center->AddChild(assetStore);

	auto layout1 = new FishGUI::SplitLayout(FishGUI::Orientation::Horizontal);
	auto layout2 = new FishGUI::SplitLayout(FishGUI::Orientation::Vertical);
	auto layout3 = new FishGUI::SplitLayout(FishGUI::Orientation::Horizontal);
	mainLayout->SetCenterLayout(layout1);
	layout1->Split(layout2, right);
	layout2->Split(layout3, bottom);
	layout3->Split(left, center);

	
	FishEditor::Init();
	auto app = new SceneViewApp(scene);
	app->Init();
	scene->m_app = app;

	win->SetOverlayDraw([app]() {
		app->Update();
	});

	FishGUI::Run();
}
catch (pybind11::error_already_set& e)
{
	std::cerr << ">>> Error! Uncaught exception:\n";
	std::cerr << e.what() << std::endl;
	return 1;
}
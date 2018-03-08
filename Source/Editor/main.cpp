#include <FishEngine/Render/GLEnvironment.hpp>	// fix  fatal error C1189: #error:  gl.h included before glew.h

#include <FishGUI/FishGUI.hpp>
#include <FishGUI/Window.hpp>

#include <UnityLayout.hpp>
#include <UnityToolBar.hpp>
#include <DirTreeWidget.hpp>
#include <FileListWidget.hpp>

#include "SceneViewApp.hpp"
#include "HierarchyWidget.hpp"
#include "GLWidget.hpp"
#include "InspectorWindow.hpp"

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
	auto win = FishGUI::NewWindow("FishEditor", 950, 600);
	auto mainLayout = new UnityLayout();
	win->SetLayout(mainLayout);

	auto toolBar = new UnityToolBar();
	mainLayout->SetToolBar(toolBar);
	auto statusBar = new FishGUI::StatusBar();
	mainLayout->SetStatusBar(statusBar);


	FishEditor::Init();
	auto app = new SceneViewApp();
	app->Init();


	auto right = new FishGUI::TabWidget("right");
	right->SetWidth(270);
	right->SetMinSize(250, 150);
	auto inspector = new InspectorWindow("Inspector");
	right->AddChild(inspector);

	auto bottom = new FishGUI::TabWidget("bottom");
	bottom->SetHeight(180);
	bottom->SetMinSize(150, 150);
	auto project = new FishGUI::Widget("Project");
	auto console = new IMWidget2("Console");
	bottom->AddChild(project);
	bottom->AddChild(console);

	auto rootNode = new FileNode(R"(D:\program\github\MonumentVally-Demo\Assets)");
	//auto rootNode = new FileNode(ApplicationFilePath());
	//	auto rootNode = new FileNode(R"(D:\program\FishGUI)");
	//	rootNode->Find("/Users/yushroom/program/FishEngine/Example/Sponza/Assets/texture");
	auto dirs = new DirTreeWidget("Dirs", rootNode);
	dirs->SetWidth(150);
	dirs->SetMinSize(100, 100);
	auto files = new UnityFileWidget("Files");
	files->SetWidth(400);
	files->SetMinSize(200, 100);
	files->GetFileListWidget()->SetRoot(rootNode);
	dirs->GetSelectionModel()->selectionChanged.connect([files](FileNode* node) {
		if (node != nullptr)
			files->GetFileListWidget()->SetRoot(node);
	});

	{
		auto layout = new FishGUI::SplitLayout(FishGUI::Orientation::Horizontal);
		project->SetLayout(layout);
		layout->Split(dirs, files);
	}


	auto left = new FishGUI::TabWidget("Left");
	left->SetWidth(200);
	left->SetMinSize(200, 150);
	auto hierarchy = new HierarchyWidget("Hierarchy");
	left->AddChild(hierarchy);

	auto center = new FishGUI::TabWidget("Center");
	center->SetWidth(500);
	center->SetMinSize(150, 150);
	auto scene = new GLWidget("Scene", app);
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

	scene->Init();


	win->SetOverlayDraw([scene]() {
		scene->DrawScene();
	});

	FishGUI::Run();
}
catch (pybind11::error_already_set& e)
{
	std::cerr << ">>> Error! Uncaught exception:\n";
	std::cerr << e.what() << std::endl;
	return 1;
}
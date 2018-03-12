#include <FishEngine/Render/GLEnvironment.hpp>	// fix  fatal error C1189: #error:  gl.h included before glew.h

#include <FishGUI/FishGUI.hpp>
#include <FishGUI/Window.hpp>

#include <UnityLayout.hpp>
#include <UnityToolBar.hpp>
#include <DirTreeWidget.hpp>
#include <FileListWidget.hpp>

//#include "SceneViewApp.hpp"
#include "HierarchyView.hpp"
//#include "GLWidget.hpp"
#include "InspectorView.hpp"

#include <FishEditor/FishEditorInternal.hpp>
#include <FishEditor/EditorApplication.hpp>
#include <FishEditor/GameView.hpp>

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


enum class EditorLayoutType
{
	e2By3,
	e4Split,
	eDefault,
	eTall,
	eWide,
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

	auto& editorApp = FishEditor::EditorApplication::GetInstance();
	editorApp.Init();

	FishEditor::Init();
//	auto app = new SceneViewApp();
//	app->Init();


	auto inspector = new InspectorView("Inspector");
	auto project = new FishGUI::Widget("Project");
	auto console = new IMWidget2("Console");
	auto hierarchy = new HierarchyView("Hierarchy");
	auto scene = new IMWidget2("Scene");
	auto game = new FishEditor::GameView();
	auto assetStore = new IMWidget2("Asset Store");

//	auto rootNode = new FileNode(R"(D:\program\github\MonumentVally-Demo\Assets)");
	auto rootNode = new FileNode(ApplicationFilePath());
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


	EditorLayoutType layoutType = EditorLayoutType::eDefault;

	if (layoutType == EditorLayoutType::eDefault)
	{
		auto right = new FishGUI::TabWidget("right");
		right->SetWidth(270);
		right->SetMinSize(250, 150);
		right->AddChild(inspector);

		auto bottom = new FishGUI::TabWidget("bottom");
		bottom->SetHeight(180);
		bottom->SetMinSize(150, 150);
		bottom->AddChild(project);
		bottom->AddChild(console);

		{
			auto layout = new FishGUI::SplitLayout(FishGUI::Orientation::Horizontal);
			project->SetLayout(layout);
			layout->Split(dirs, files);
		}

		auto left = new FishGUI::TabWidget("Left");
		left->SetWidth(200);
		left->SetMinSize(200, 150);
		left->AddChild(hierarchy);

		auto center = new FishGUI::TabWidget("Center");
		center->SetWidth(500);
		center->SetMinSize(150, 150);
		center->AddChild(scene);
		center->AddChild(game);
		center->AddChild(assetStore);
		center->SetActiveTab(1);

		auto layout1 = new FishGUI::SplitLayout(FishGUI::Orientation::Horizontal);
		auto layout2 = new FishGUI::SplitLayout(FishGUI::Orientation::Vertical);
		auto layout3 = new FishGUI::SplitLayout(FishGUI::Orientation::Horizontal);
		mainLayout->SetCenterLayout(layout1);
		layout1->Split(layout2, right);
		layout2->Split(layout3, bottom);
		layout3->Split(left, center);

	}
	else if (layoutType == EditorLayoutType::e2By3)
	{
		auto tab1 = new FishGUI::TabWidget("1");
		auto tab2 = new FishGUI::TabWidget("2");
		auto tab3 = new FishGUI::TabWidget("3");
		auto tab4 = new FishGUI::TabWidget("4");
		auto tab5 = new FishGUI::TabWidget("5");

		tab1->AddChild(inspector);
		tab2->AddChild(project);
		tab3->AddChild(hierarchy);
		tab4->AddChild(scene);
		tab5->AddChild(game);

		tab1->SetMinSize(270, 150);
		tab1->SetMinSize(250, 150);
		tab2->SetMinSize(150, 150);
		tab3->SetMinSize(150, 150);
		tab4->SetMinSize(150, 150);

		{
			auto layout = new FishGUI::SplitLayout(FishGUI::Orientation::Vertical);
			project->SetLayout(layout);
			layout->Split(dirs, files);
			files->SetMinSize(150, 150);
		}

		auto layout1 = new FishGUI::SplitLayout(FishGUI::Orientation::Horizontal);
		auto layout2 = new FishGUI::SplitLayout(FishGUI::Orientation::Horizontal);
		auto layout3 = new FishGUI::SplitLayout(FishGUI::Orientation::Horizontal);
		auto layout4 = new FishGUI::SplitLayout(FishGUI::Orientation::Vertical);
		mainLayout->SetCenterLayout(layout1);
		layout1->Split(layout2, tab1);
		layout2->Split(layout3, tab2);
		layout3->Split(layout4, tab3);
		layout4->Split(tab4, tab5);
	}


	game->Init();
//	editorApp.Start();
	toolBar->OnRun.connect([&editorApp](){
		editorApp.Play();
	});
	
	toolBar->OnStop.connect([&editorApp](){
		editorApp.Stop();
	});
	
	toolBar->OnPause.connect([&editorApp](){
		editorApp.Pause();
	});
	
	toolBar->OnResume.connect([&editorApp](){
		editorApp.Resume();
	});
	
//	toolBar->OnNextFrame.connect([&editorApp]{
//		editorApp.NextFrame();
//	});

	win->SetOverlayDraw([&editorApp]() {
		editorApp.Update();
	});

	FishGUI::Run();
	FishEngine::Clean();
	return 0;
}
catch (pybind11::error_already_set& e)
{
	std::cerr << ">>> Error! Uncaught exception:\n";
	std::cerr << e.what() << std::endl;
	return 1;
}

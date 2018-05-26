#include "ProjectView.hpp"

#include <iostream>

#include <FishGUI/ModelView/TreeWidget.hpp>
#include <FishGUI/Icon.hpp>
#include <FishGUI/ModelView/ListWidget.hpp>
#include <FishGUI/Utils.hpp>

#include <FishEditor/AssetDatabase.hpp>
#include <FishEditor/AssetImporter.hpp>
#include <FishEditor/EditorApplication.hpp>
#include <FishEditor/FileNode.hpp>

#include <FishEditor/Selection.hpp>

using FishEditor::FileNode;

class DirTreeModel : public FishGUI::TItemModel<FileNode*>
{
    typedef FileNode* T;
public:
    T Parent(T child) const override
    {
        return child->parent;
    }

    inline FileNode* ChildAt(int index, FileNode* parent) const override
    {
        return parent->subdirs[index];
    }

    inline int ChildCount(FileNode* parent) const override
    {
        if (parent == nullptr)
            return 0;
        return static_cast<int>(parent->subdirs.size());
    }

    inline std::string Text(FileNode* item) const override
    {
        return item->fileName;
    }

    inline const FishGUI::Icon*	Icon(FileNode* item) const override
    {
        static char icon[8];
        static FishGUI::FontIcon folderIcon { CodePointToUTF8(0xe930, icon), 12.0f, "ui" };
        return &folderIcon;
    }
};


class DirTreeWidget : public FishGUI::TreeWidget<FileNode*>
{
public:
    typedef FishGUI::TreeWidget<FileNode*> Super;

    DirTreeWidget(const char* name) : Super(name)
    {
        m_model = new DirTreeModel();
        m_selectionModel.SetMode(FishGUI::SelectionMode::SingleSelection);
//        m_root = root;
//        m_unfolded.insert(m_root);
//        m_selectionModel.SelectItem(m_root);
    }
	
	void SetRootDir(FileNode* root)
	{
		m_root = root;
		m_unfolded.insert(m_root);
		m_selectionModel.SelectItem(m_root);
	}
};


class FileListModel : public FishGUI::TItemModel<FileNode*>
{
    typedef FileNode* T;
public:
    T Parent(T child) const override
    {
        return child->parent;
    }

    int ChildCount(T item) const override
    {
        //assert(item == m_root);
        return static_cast<int>( item->subdirs.size() + item->files.size());
    }

    T ChildAt(int index, T parent) const override
    {
        //assert(parent == m_root);
        if (index < parent->subdirs.size())
            return parent->subdirs[index];
        return parent->files[index - parent->subdirs.size()];
    }

    std::string Text(T item) const override
    {
        return item->fileName;
    }

    const FishGUI::Icon* Icon(T item) const override
    {
//        constexpr int dirIconCP = 0xf115;
//        constexpr int fileIconCP = 0xf016;
//        char icon[8];
//        static FishGUI::FontIcon dirIcon{ CodePointToUTF8(dirIconCP, icon), 64.f, "ui" };
//        static FishGUI::FontIcon fileIcon{ CodePointToUTF8(fileIconCP, icon), 64.f, "ui" };
//        return item->isDir ? &dirIcon : &fileIcon;

		// TODO: delete
#ifdef _WIN32
		std::string icons_root = R"(D:\program\FishEngine-Experiment\Assets\Textures\Icons\)";
#else
		std::string icons_root = "/Users/yushroom/program/FishEngine-Experiment/Assets/Textures/Icons/";
#endif
		static auto folder_icon = FishGUI::ImageIcon::FromFile(icons_root + "Folder@64.png");
		static auto default_icon = FishGUI::ImageIcon::FromFile(icons_root+"DefaultAsset@64.png");
//		static auto audioclip_icon = FishGUI::ImageIcon::FromFile(icons_root+"AudioClip@64.png");
//		static auto font_icon = FishGUI::ImageIcon::FromFile(icons_root+"Font@64.png");
		static auto material_icon = FishGUI::ImageIcon::FromFile(icons_root+"Material@64.png");
		static auto prefab_icon = FishGUI::ImageIcon::FromFile(icons_root+"PrefabNormal@64.png");
		static auto prefabModel_icon = FishGUI::ImageIcon::FromFile(icons_root+"PrefabModel@64.png");
		static auto shader_icon = FishGUI::ImageIcon::FromFile(icons_root+"Shader@64.png");
		static auto scene_icon = FishGUI::ImageIcon::FromFile(icons_root+"SceneAsset@64.png");
		static auto cs_icon = FishGUI::ImageIcon::FromFile(icons_root+"cs Script@64.png");
		
		if (item->isDir)
			return folder_icon;
		
		auto ext = item->path.extension();
		if (ext == ".prefab")
			return prefab_icon;
		else if (ext == ".fbx")
			return prefabModel_icon;
		else if (ext == ".mat")
			return material_icon;
		else if (ext == ".shader")
			return shader_icon;
		else if (ext == ".unity")
			return scene_icon;
		else if (ext == ".cs")
			return cs_icon;
		return default_icon;
    }
};

class FileListWidget : public FishGUI::ListWidget<FileNode*>
{
    typedef ListWidget<FileNode*> Super;
public:
    explicit FileListWidget(const char* name) : Super(name)
    {
        m_model = new FileListModel();
        m_selectionModel.OnSelectionChanged.connect([](FileNode* selected) {
            if (selected != nullptr)
                std::cout << selected->path << std::endl;
        });
    }
};


class UnityFileWidget : public FishGUI::Widget
{
public:
    explicit UnityFileWidget(const char* name) : Widget(name), m_listWidget("")
    {
        m_listWidget.GetSelectionModel()->OnSelectionChanged.connect([this](FileNode* selected) {
			this->m_selectedFile = selected;
			this->OnSelectedFileChanged(selected);
        });
    }

    void Draw() override
    {
        constexpr int bottomLabelHeight = 20;
        auto r = m_rect;
        m_listWidget.SetRect(r.x, r.y, r.width, r.height - bottomLabelHeight);
        m_listWidget.BindAndDraw();
        std::string filename;
        if (m_selectedFile != nullptr)
            filename = Path(m_selectedFile->path).filename().string();
        r.y = r.bottom() - bottomLabelHeight;
        r.height = bottomLabelHeight;
        FishGUI::DrawRect(FishGUI::Context::GetInstance().m_drawContext, r, nvgRGB(221, 221, 221));
        FishGUI::Label(filename, r);
    }

    FileListWidget * GetFileListWidget() { return &m_listWidget; }

	boost::signals2::signal<void(FileNode*)> OnSelectedFileChanged;

protected:
    FileListWidget m_listWidget;
    FileNode* m_selectedFile = nullptr;
};


void ProjectView::UpdateRoot()
{
    auto root = FishEditor::AssetDatabase::AssetRootDir();
    this->m_DirTree->SetRootDir(root);
    this->m_FileList->GetFileListWidget()->SetRoot(root);
}

ProjectView::ProjectView() : Widget("Project")
{
    m_DirTree = new DirTreeWidget("Dirs");
    m_FileList = new UnityFileWidget("Files");
	m_DirTree->SetWidth(150);
	m_DirTree->SetMinSize(100, 100);
	m_FileList->SetWidth(400);
	m_FileList->SetMinSize(200, 100);
	m_DirTree->GetSelectionModel()->OnSelectionChanged.connect([this](FileNode* node) {
		if (node != nullptr)
			this->m_FileList->GetFileListWidget()->SetRoot(node);
	});
    auto layout = new FishGUI::SplitLayout(FishGUI::Orientation::Horizontal);
    this->SetLayout(layout);
    layout->Split(m_DirTree, m_FileList);
	UpdateRoot();
	
	FishEditor::EditorApplication::GetInstance().OnProjectOpened.connect([this](){
		this->UpdateRoot();
    });

	m_FileList->OnSelectedFileChanged.connect([](FileNode* node) {
		if (node != nullptr)
		{
//			if (!node->guid.empty())
//			{
//				//auto path = FishEditor::AssetDatabase::GUIDToAssetPath(node->guid);
//				auto importer = FishEditor::AssetImporter::GetByGUID(node->guid);
//				FishEditor::Selection::SetActiveObject(importer);
//			}
		}
		else
		{
			FishEditor::Selection::SetActiveObject(nullptr);
		}
	});
}

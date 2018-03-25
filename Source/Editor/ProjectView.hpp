#pragma once

#include <FishGUI/Widget.hpp>

class DirTreeWidget;
class UnityFileWidget;

class ProjectView : public FishGUI::Widget
{
public:
    ProjectView();

    void UpdateRoot();
//    DirTreeWidget* GetDirTree() const { return m_DirTree; }
//    UnityFileWidget* GetFileList() const { return m_FileList; }

private:
	DirTreeWidget * m_DirTree = nullptr;
    UnityFileWidget * m_FileList = nullptr;
};

#pragma once

#include <FishGUI/Widget.hpp>


class UnityLayout : public FishGUI::Layout
{
public:
	virtual void PerformLayout(const FishGUI::Rect& rect) override
	{
		auto r = rect;
		if (m_ToolBar != nullptr)
		{
			m_ToolBar->SetRect(0, 0, r.width, m_ToolBar->GetHeight());
			m_ToolBar->BindAndDraw();
		}
		
		if (m_StatusBar != nullptr)
		{
			m_StatusBar->SetRect(0, r.height - m_StatusBar->GetHeight(), r.width, m_StatusBar->GetHeight());
			m_StatusBar->BindAndDraw();
		}
		if (m_ToolBar != nullptr)
		{
			r.height -= m_ToolBar->GetHeight();
			r.y += m_ToolBar->GetHeight();
		}
		if (m_StatusBar != nullptr)
		{
			r.height -= m_StatusBar->GetHeight();
		}
		
		if (m_CenterLayout != nullptr)
		{
			m_CenterLayout->PerformLayout(r);
		}
	}
	
	void SetCenterLayout(Layout* layout)
	{
		m_CenterLayout = layout;
	}
	
	void SetToolBar(FishGUI::ToolBar* toolBar)
	{
		m_ToolBar = toolBar;
	}
	
	void SetStatusBar(FishGUI::StatusBar* statusBar)
	{
		m_StatusBar = statusBar;
	}
	
	FishGUI::ToolBar* GetToolBar()
	{
		return m_ToolBar;
	}
	
	FishGUI::StatusBar* GetStatusBar()
	{
		return m_StatusBar;
	}
	
protected:
	FishGUI::ToolBar* 		m_ToolBar 	= nullptr;
	FishGUI::StatusBar* 	m_StatusBar = nullptr;
	Layout* 				m_CenterLayout = nullptr;
};

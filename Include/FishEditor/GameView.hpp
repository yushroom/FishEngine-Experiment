#pragma once

#include "EditorWindow.hpp"

#include <FishGUI/Render/FrameBuffer.hpp>
#include <FishGUI/Widget.hpp>

namespace FishEditor
{
//	class GameView : public EditorWindow
	class GameView : public FishGUI::IMWidget
	{
	public:
		GameView() : IMWidget("Game")
		{
			assert(s_Current == nullptr);
			s_Current = this;
		}
		
		void Init();
		
		virtual void DrawImpl() override;
		
		void Resize(int width, int height);
		
		static GameView* GetCurrent() { return s_Current; }
		
//	private:
		FishGUI::FrameBuffer m_FrameBuffer;
		
		static GameView* s_Current;
	};
}

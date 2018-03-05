#include <FishEngine/Screen.hpp>
#include <FishEngine/GameApp.hpp>

namespace FishEngine
{
	int     Screen::s_width             = 128;
	int     Screen::s_height            = 128;
//	float   Screen::s_dpi               = 1.0f;
	float   Screen::s_pixelsPerPoint    = 1.0f;
	
	void Screen::SetResolution(int width, int height, bool fullscreen, int preferredRefreshRate)
	{
//		s_width = width;
//		s_height = height;
		auto app = GameApp::GetCurrent();
		app->Resize(width, height);
	}
}

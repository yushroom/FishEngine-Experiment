#include <FishEngine/Screen.hpp>

namespace FishEngine
{
	int     Screen::s_Width             = 128;
	int     Screen::s_Height            = 128;
//	float   Screen::s_dpi               = 1.0f;
	float   Screen::s_PixelsPerPoint    = 1.0f;
	
	boost::signals2::signal<void(int, int)> Screen::OnResolutionChange;
	
	void Screen::SetResolution(int width, int height, bool fullscreen, int preferredRefreshRate)
	{
		if (s_Width == width && s_Height == height)
			return;
		s_Width = width;
		s_Height = height;
		Screen::OnResolutionChange(s_Width, s_Height);
	}
}

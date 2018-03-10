#include <FishEngine/Screen.hpp>

namespace FishEngine
{
	int     Screen::s_width             = 128;
	int     Screen::s_height            = 128;
//	float   Screen::s_dpi               = 1.0f;
	float   Screen::s_pixelsPerPoint    = 1.0f;
	
	boost::signals2::signal<void(int, int)> Screen::OnResolutionChange;
	
	void Screen::SetResolution(int width, int height, bool fullscreen, int preferredRefreshRate)
	{
		if (s_width == width && s_height == height)
			return;
		s_width = width;
		s_height = height;
		Screen::OnResolutionChange(s_width, s_height);
	}
}

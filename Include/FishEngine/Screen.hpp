#pragma once

#include "FishEngine.hpp"
#include <boost/signals2/signal.hpp>

//namespace FishGame { class GameApp; }

namespace FishEngine
{
	class FE_EXPORT Screen
	{
	public:
		Screen() = delete;
		
		// The current width of the screen window in pixels (Read Only).
		static int GetWidth() { return s_width; }

		// The current height of the screen window in pixels (Read Only).
		static int GetHeight() { return s_height; }

		static float GetAspect() { return static_cast<float>(s_width) / static_cast<float>(s_height); }

		// The current DPI of the screen / device (Read Only).
//		static float dpi() { return s_dpi; }
		
		static float GetPixelsPerPoint() { return s_pixelsPerPoint; }
//		static void setPixelsPerPoint(const float value) { s_pixelsPerPoint = value; }
		
		static void SetResolution(int width, int height, bool fullscreen, int preferredRefreshRate = 0);
		
		static boost::signals2::signal<void(int, int)> OnResolutionChange;
		
	private:
		friend class GameApp;
		static int s_width;
		static int s_height;
//		static float s_dpi;
		
		// This value is the number of screen pixels per point of interface space. For instance, 2.0 on retina displays.
		static float s_pixelsPerPoint;
	};
}

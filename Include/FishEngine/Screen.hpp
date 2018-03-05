#pragma once

#include "FishEngine.hpp"

//namespace FishGame { class GameApp; }

namespace FishEngine
{
	class FE_EXPORT Screen
	{
	public:
		Screen() = delete;

		static void set(int width, int height)
		{
			s_width = width;
			s_height = height;
		}
		
		// The current width of the screen window in pixels (Read Only).
		static int width() { return s_width; }

		// The current height of the screen window in pixels (Read Only).
		static int height() { return s_height; }

		static float aspect() { return static_cast<float>(s_width) / static_cast<float>(s_height); }

		// The current DPI of the screen / device (Read Only).
//		static float dpi() { return s_dpi; }
		
		static float pixelsPerPoint() { return s_pixelsPerPoint; }
//		static void setPixelsPerPoint(const float value) { s_pixelsPerPoint = value; }
		
		static void SetResolution(int width, int height, bool fullscreen, int preferredRefreshRate = 0);
		
	private:
		friend class GameApp;
		static int s_width;
		static int s_height;
//		static float s_dpi;
		
		// This value is the number of screen pixels per point of interface space. For instance, 2.0 on retina displays.
		static float s_pixelsPerPoint;
	};
}

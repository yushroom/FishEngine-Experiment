#pragma once

#include "Object.hpp"

namespace FishEngine
{
	enum class ShadowResolution
	{
		Low,        // Low shadow map resolution.
		Medium,     // Medium shadow map resolution.
		High,       // High shadow map resolution.
		VeryHigh,   // Very high shadow map resolution.
	};

	enum class ShadowQuality
	{
		Disable = 0,	// Disable Shadows.
		HardOnly= 1,	// Hard Shadows Only.
		All = 2,		// Hard and Soft Shadows.
	};

	class QualitySettings final : public Object
	{
	public:
		DeclareObject(QualitySettings, 47);
		QualitySettings() : Object(ClassID, ClassName)
		{
		}

		static QualitySettings& GetInstance()
		{
			static QualitySettings instance;
			return instance;
		}

		void setShadowDistance(float shadowDistance)
		{
			s_shadowDistance = shadowDistance;
		}

		float shadowDistance()
		{
			return s_shadowDistance;
		}

		static uint32_t CalculateShadowMapSize();

	private:
		// Shadows	This determines which type of shadows should be used.The available options are Hard and Soft Shadows, Hard Shadows Only and Disable Shadows.

		// Shadow resolution	Shadows can be rendered at several different resolutions : Low, Medium, High and Very High.The higher the resolution, the greater the processing overhead.
		ShadowResolution m_shadowResolution = ShadowResolution::High;

		// Shadow Projection	There are two different methods for projecting shadows from a directional light.Close Fit renders higher resolution shadows but they can sometimes wobble slightly if the camera moves.Stable Fit renders lower resolution shadows but they don't wobble with camera movements.


		// Shadow Cascades	The number of shadow cascades can be set to zero, two or four.A higher number of cascades gives better quality but at the expense of processing overhead(see Directional Light Shadows for further details).
		int m_shadowCascades = 4;

		// Shadow Distance	The maximum distance from camera at which shadows will be visible.Shadows that fall beyond this distance will not be rendered.
		float s_shadowDistance = 150.0f;

		// Shadow Near Plane Offset	Offset shadow near plane to account for large triangles being distorted by shadow pancaking.
		float m_shadowNearPlaneOffset = 2.0f;
	};
}

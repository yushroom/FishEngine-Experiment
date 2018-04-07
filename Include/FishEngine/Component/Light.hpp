#ifndef Light_hpp
#define Light_hpp

#include "Behaviour.hpp"
#include "../Color.hpp"
#include "../Render/RenderTexture.hpp"
#include "../Render/RenderBuffer.hpp"
#include "../Render/RenderTarget.hpp"
//#include "Common.hpp"
#include "../Math/Matrix4x4.hpp"

// reference:
// https://docs.unity3d.com/Manual/Lighting.html
// https://docs.unity3d.com/Manual/class-Light.html
// https://docs.unity3d.com/Manual/LightPerformance.html
// https://docs.unity3d.com/Manual/DirLightShadows.html

namespace FishEngine
{
	enum class LightType
	{
		Spot,
		Directional,
		Point,
		Area,
	};

//	class LayeredDepthBuffer;
//	class RenderTarget;

	class FE_EXPORT Light : public Behaviour
	{
	public:
		DeclareObject(Light, 108);
		
		Light();
		
		~Light();

//		static LightPtr Create();

		//        static std::list<std::weak_ptr<Light>>& lights()
		//        {
		//            return m_lights;
		//        }

//		static LightPtr mainLight()
//		{
//			LightPtr ret = nullptr;
//			while (!m_lights.empty())
//			{
//				if (m_lights.front().expired())
//				{
//					m_lights.pop_front();
//				}
//				else
//				{
//					ret = m_lights.front().lock();
//					break;
//				}
//			}
//			return ret;
//		}

		float shadowNearPlane() const
		{
			return m_shadowNearPlane;
		}

//		virtual void OnDrawGizmos() override;
//		virtual void OnDrawGizmosSelected() override;

		static void ResizeShadowMaps();

	private:
		friend class Scene;
		//friend class FishEditor::EditorRenderSystem;
		friend class RenderSystem;
//		friend class FishEditor::Inspector;
		friend class MeshRenderer;
		friend class SkinnedMeshRenderer;
		friend class Graphics;
		friend class Pipeline;

		// The current type of light. Possible values are Directional, Point, Spot and Area
		LightType m_type = LightType::Directional;

		// How far light is emitted from the center of the object (Point and Spot lights only).
		float m_range = 10.0f;

		// Determines the angle(in degrees) at the base of a spot lights cone(Spot light only).
		float m_spotAngle = 30.0f;

		// The color of the light emitted.
		Color m_color = Color::white;

		// Brightness of the light. The default value for a Point, Spot or Area light is 1 but for a Directional light, it is 0.5.
		float m_intensity = 1.0f;

		float m_shadowBias = 0.05f;
		float m_shadowNormalBias = 0.4f;
		float m_shadowNearPlane = 0.2f;
		float m_shadowStrength = 1.0f;

	public:
		LayeredDepthBuffer* m_shadowMap;
		RenderTarget* m_renderTarget;
		Matrix4x4 m_viewMatrixForShadowMap[4];
		Matrix4x4 m_projectMatrixForShadowMap[4];
		Vector4 m_cascadesNear;
		Vector4 m_cascadesFar;
		Vector4 m_cascadesSplitPlaneNear;
		Vector4 m_cascadesSplitPlaneFar;
	};
}

#endif // Light_hpp

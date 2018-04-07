#pragma once

#include "../Math/Matrix4x4.hpp"
#include "ShaderVariables.hpp"
#include <stack>

namespace FishEngine
{
	class Camera;
	class Light;
	class RenderTarget;

	class Pipeline
	{
		public:
		Pipeline() = delete;

		static void StaticInit();

		static void BindCamera(Camera* camera);
		static void BindLight(Light* light);

		static void UpdatePerDrawUniforms(const Matrix4x4& modelMatrix);

		static void UpdateBonesUniforms(const std::vector<Matrix4x4>& bones);

		static RenderTarget* CurrentRenderTarget()
		{
			return s_renderTargetStack.top();
		}

		static void PushRenderTarget(RenderTarget* renderTarget);

		static void PopRenderTarget();

		// screen coord:
		// (0, 1)    (1, 1)
		//    -----------
		//    |         |
		//    |         |
		//    |         |
		//    -----------
		// (0, 0)     (0, 1)
		//
		// OpenGL Clip
		// (-1, 1)    (1, 1)
		//    -----------
		//    |         |
		//    |         |
		//    |         |
		//    -----------
		// (-1, -1)    (1, -1)
		static void DrawRect();

		static constexpr unsigned int PerCameraUBOBindingPoint = 0;
		static constexpr unsigned int PerDrawUBOBindingPoint = 1;
		static constexpr unsigned int LightingUBOBindingPoint = 2;
		static constexpr unsigned int BonesUBOBindingPoint = 3;

		private:
		static unsigned int         s_perCameraUBO;
		static unsigned int         s_perDrawUBO;
		static unsigned int         s_lightingUBO;
		static unsigned int         s_bonesUBO;
		static PerCameraUniforms    s_perCameraUniforms;
		static PerDrawUniforms      s_perDrawUniforms;
		static LightingUniforms     s_lightingUniforms;
		//static Bones        s_bonesUniformData;

		static RenderTarget*      s_currentRenderTarget;

		static std::stack<RenderTarget*> s_renderTargetStack;
	};
}

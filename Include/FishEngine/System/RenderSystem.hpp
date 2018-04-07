#pragma once

namespace FishEngine
{
	class RenderTarget;
	class RenderBuffer;
	class ColorBuffer;
	class DepthBuffer;
	class LayeredDepthBuffer;
	class Shader;

	class RenderSystem
	{
	public:
		RenderSystem(RenderSystem&) = delete;
		RenderSystem& operator=(RenderSystem&) = delete;

		static RenderSystem& GetInstance()
		{
			static RenderSystem instance;
			return instance;
		}
		
		void Update();
		
	private:
		RenderSystem();

		RenderTarget* m_MainRenderTarget;
		ColorBuffer*  m_MainColorBuffer;
		DepthBuffer* m_MainDepthBuffer;

		// depth pass
		RenderTarget* m_DepthPassRT;
		DepthBuffer* m_SceneDepth;
		Shader* m_RenderDepthShader;

		// csm pass
//		RenderTarget* m_ShadowMapRT;
		Shader* m_CSMShader;

		// collect shadows
		RenderTarget* m_CollectShadowsRT;
		ColorBuffer* m_ScreenSpaceShadowMap;
		Shader* m_CollectShadowsShader;

		// add shadow
		RenderTarget* m_AddShadowRT;
		ColorBuffer* m_AddShadowColorBuffer;
		Shader* m_AddShadowShader;

	};
}

#pragma once

#include <vector>

namespace FishEngine
{
	class RenderTarget;
	class RenderBuffer;
	class ColorBuffer;
	class DepthBuffer;
	class LayeredDepthBuffer;
	class Shader;

	class GameObject;
	class Mesh;
	class Material;
	class Renderer;

	struct RenderObject
	{
		GameObject* gameObject;
		Renderer*	renderer;
		Mesh*		mesh;
		Material*	material;

		RenderObject(GameObject* gameObject,
				Renderer*	renderer,
				Mesh*		mesh,
				Material*	material) : gameObject(gameObject), renderer(renderer), mesh(mesh), material(material)
		{

		}
	};

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

		void GetRenderObjects();

		std::vector<RenderObject> m_RenderObjects;

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

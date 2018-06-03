#include <FishEngine/System/RenderSystem.hpp>

#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/Component/Camera.hpp>
#include <FishEngine/Component/Light.hpp>
#include <FishEngine/Component/MeshFilter.hpp>
#include <FishEngine/Component/MeshRenderer.hpp>
#include <FishEngine/Component/SkinnedMeshRenderer.hpp>
#include <FishEngine/Render/Graphics.hpp>
#include <FishEngine/Render/GLEnvironment.hpp>
#include <FishEngine/Render/Shader.hpp>
#include <FishEngine/Scene.hpp>
#include <FishEngine/Render/Material.hpp>
#include <FishEngine/Render/Mesh.hpp>
#include <FishEngine/Render/Pipeline.hpp>

#include <FishEditor/Path.hpp>

#include <FishEngine/Gizmos.hpp>

#include <algorithm>

#ifdef _WIN32
#undef near
#undef far
#endif


#include <FishEngine/Animation/Animation.hpp>
#include <FishEngine/Gizmos.hpp>

namespace FishEngine
{

	void RenderShadowMap(Shader* shader, Camera* camera, Light* light, std::vector<RenderObject> const& renderObjects)
	{
		if (light == nullptr)
		{
			return;
		}

#define DEBUG_SHADOW 1
		auto    camera_to_world = camera->GetCameraToWorldMatrix();
		float   near = camera->GetNearClipPlane();
		//float   far = camera->farClipPlane();
		// TODO: QualitySettings
		float  far = std::min( 150.0f, camera->GetFarClipPlane() );

		Vector3 light_dir = light->GetTransform()->GetForward();
		Frustum total_frustum = camera->GetFrustum();

		constexpr float splits[] = { 0, 1.0f / 15.0f, 3.0f / 15.0f, 7.0f / 15.0f, 1 };

		for (int splitInex = 0; splitInex < 4; ++splitInex)
		{
#if 0
			// From GPU Gem 3. Chap 10 "Practical Split Scheme".
			const float split_lambda = 0.5f;
			float split_near = near;
			if (i > 0)
				split_near = Mathf::Lerp(near + i / 4.0f * (far - near), near * Mathf::Pow(far / near, i / 4.0f), split_lambda);
			float split_far = far;
			if (i < 4 - 1)
				split_far = Mathf::Lerp(near + (i + 1) / 4.0f * (far - near), near*Mathf::Pow(far / near, (i + 1) / 4.0f), split_lambda);
#else
			// From Unity
			// 0:    0 ~ 1/15,  1/15
			// 1: 1/15 ~ 3/15,  2/15
			// 2: 3/15 ~ 7/15,  4/15
			// 3: 7/15 ~ 1,     8/15
			float split_near = Mathf::Lerp(near, far, splits[splitInex]);
			float split_far = Mathf::Lerp(near, far, splits[splitInex+1]);
#endif

			Frustum frustum = total_frustum;
			frustum.minRange = split_near;
			frustum.maxRange = split_far;

//			Gizmos::setMatrix(camera_to_world);
//			Gizmos::setColor(Color::cyan * (i / 3.0f));
//			Gizmos::DrawFrustum(frustum);
//			Gizmos::setMatrix(Matrix4x4::identity);

			Vector3 view_corners[8];
			frustum.getLocalCorners(view_corners);
			Vector3 world_corners[8];
			for (int i = 0; i < 8; ++i)
			{
				world_corners[i] = camera_to_world.MultiplyPoint(view_corners[i]);
			}

			Vector3 split_centroid(0, 0, 0);
			for (auto& c : world_corners)
			{
				split_centroid += c;
			}
			split_centroid /= 8.0f;

			float dist = Mathf::Max(split_far - split_near, Vector3::Distance(world_corners[4], world_corners[5]));
			auto eye_pos = split_centroid - light_dir * dist;
//			Gizmos::DrawWireSphere(eye_pos, 0.5f);
			Matrix4x4 world_to_light = Matrix4x4::LookAt(eye_pos, split_centroid, Vector3::up);

			Bounds aabb;    // the bounding box of view frustum in light's local space
			for (int i = 0; i < 8; ++i)
			{
				view_corners[i] = world_to_light.MultiplyPoint(world_corners[i]);
				aabb.Encapsulate(view_corners[i]);
			}

#define CSM_SPLIT_SPHERE
#ifdef CSM_SPLIT_SPHERE
			float sphereRadius = 0.0f;
			for (auto& c : world_corners)
			{
				float dist = Vector3::Distance(c, split_centroid);
				sphereRadius = std::max(sphereRadius, dist);
			}

			sphereRadius = std::ceil(sphereRadius * 16.0f) / 16.0f;
			Vector3 maxExtents(sphereRadius, sphereRadius, sphereRadius);
			Vector3 minExtents = -maxExtents;
			Vector3 cascadeExtents = maxExtents - minExtents;
#endif
			constexpr float near_offset = 10.0f;
			constexpr float far_offset = 20.0f;
			auto min_p = aabb.min();
			auto max_p = aabb.max();

//			Gizmos::setColor(Color::red * (i / 3.0f));
//			Gizmos::setMatrix(world_to_light.inverse());
//			Gizmos::DrawWireCube(aabb.center(), aabb.size());
//			Gizmos::setMatrix(Matrix4x4::identity);

			float z_near = min_p.z;
			float z_far = max_p.z;
			light->m_cascadesNear[splitInex] = z_near - near_offset;
			light->m_cascadesFar[splitInex] = z_far + far_offset;

			//Debug::Log("sphereRadius: %lf", sphereRadius);
			auto shadowCameraPos = -light_dir * sphereRadius + split_centroid;
			auto shadowView = Matrix4x4::LookAt(shadowCameraPos, split_centroid, Vector3::up);
			auto shadowProj = Matrix4x4::Ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, 0, cascadeExtents.z);

			const float halfShadowMapSize = 0.5f * light->m_shadowMap->width();
			auto shadowMatrix = shadowProj * shadowView;
			Vector3 shadowOrigin = shadowMatrix.MultiplyPoint3x4(Vector3::zero);
			shadowOrigin *= halfShadowMapSize;
			Vector3 roundedOrigin{ std::round(shadowOrigin.x), std::round(shadowOrigin.y), std::round(shadowOrigin.z) };
			Vector3 roundOffset = roundedOrigin - shadowOrigin;
			roundOffset /= halfShadowMapSize;
			roundOffset.z = 0.0f;

			shadowProj[0][3] += roundOffset.x;
			shadowProj[1][3] += roundOffset.y;

			light->m_projectMatrixForShadowMap[splitInex] = shadowProj;
			light->m_viewMatrixForShadowMap[splitInex] = shadowView;

			light->m_cascadesSplitPlaneNear[splitInex] = split_near;
			light->m_cascadesSplitPlaneFar[splitInex] = split_far;
		}

////		auto shadow_map_material = Material::builtinMaterial("CascadedShadowMap");
//		static Material* shadow_map_material = nullptr;
//		if (shadow_map_material == nullptr)
//		{
//			shadow_map_material = new Material;
//			auto str = FishEditor::ReadFileAsString("/Users/yushroom/program/FishEngine-Experiment/Assets/Shaders/CascadedShadowMap.shader");
//			auto shader = Shader::FromString(str, true);
//			shadow_map_material->SetShader(shader);
//		}

		Pipeline::BindLight(light);

		auto shadowMap = light->m_shadowMap;
		Pipeline::PushRenderTarget(light->m_renderTarget);
		shader->Use();

		glViewport(0, 0, shadowMap->width(), shadowMap->height());
		glClear(GL_DEPTH_BUFFER_BIT);

		glFrontFace(GL_CW);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_CLAMP);

		for (auto&& ro : renderObjects)
		{
			if (!ro.renderer->GetEnabled() || ro.renderer->GetCastShadows() == ShadowCastingMode::Off)
				continue;
			auto modelMat = ro.gameObject->GetTransform()->GetLocalToWorldMatrix();
			Pipeline::UpdatePerDrawUniforms(modelMat);
			ro.mesh->Render(-1);
		}

		glDisable(GL_DEPTH_CLAMP);
		Pipeline::PopRenderTarget();
#undef DEBUG_SHADOW
	}


	Shader* ShaderFromFile(const char* path, bool useGeometryShader = false)
	{
#if _WIN32
		std::string root = R"(D:\program\FishEngine-Experiment\Assets\Shaders\)";
#else
		std::string root = "/Users/yushroom/program/FishEngine-Experiment/Assets/Shaders/";
#endif
		auto str = FishEditor::ReadFileAsString(root + path);
		return Shader::FromString(str, useGeometryShader);
	}



	void ShowShadowMap(Texture* shadowMap)
	{
		static Shader* shader = nullptr;
		if (shader == nullptr)
		{
			shader = ShaderFromFile("DisplayCSM.shader");
		}
		glDisable(GL_CULL_FACE);
		shader->Use();
		shader->BindTexture("_MainTex", shadowMap);
		auto quad = Mesh::m_Quad;
		quad->Render(-1);
		glCheckError();
	}


	void DrawTexture(Texture* shadowMap)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		static Shader* shader = nullptr;
		if (shader == nullptr)
		{
			shader = ShaderFromFile("DrawQuad.shader");
		}
		glDisable(GL_CULL_FACE);
		shader->Use();
		shader->BindTexture("_MainTex", shadowMap);
		auto quad = Mesh::m_Quad;
		quad->Render(-1);
		glCheckError();
	}

	void CollectShadows(Shader* shader, LayeredDepthBuffer* shadowMap, DepthBuffer* sceneDepth)
	{
		glDepthFunc(GL_ALWAYS);
		glDepthMask(GL_FALSE);
		glDisable(GL_CULL_FACE);
		shader->Use();
		shader->BindTexture("SceneDepthTexture", sceneDepth);
		shader->BindTexture("CascadedShadowMap", shadowMap);
		float shadowMapSize = static_cast<float>( shadowMap->width() );
		float shadowMapTexelSize = 1.0f / shadowMapSize;
		shader->BindUniform("_ShadowMapTexture_TexelSize", Vector4(shadowMapTexelSize, shadowMapTexelSize, shadowMapSize, shadowMapSize));

		glCheckError();
		auto quad = Mesh::m_ScreenAlignedQuad;
		quad->Render(-1);
		glCheckError();
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glUseProgram(0);
	}

	void PostProcessShadow(Texture* mainColorTexture, Texture* screenShadowMap)
	{
		static Shader* shader = nullptr;
		if (shader == nullptr)
		{
			shader = ShaderFromFile("PostProcessShadow.shader");
		}
		shader->Use();
		// add shadow
		glDepthFunc(GL_ALWAYS);
		glDepthMask(GL_FALSE);
		auto quad = Mesh::m_ScreenAlignedQuad;
		shader->BindTexture("_MainTex", mainColorTexture);
		shader->BindTexture("ScreenShadow", screenShadowMap);
		quad->Render(-1);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
	}


	void AddShadow(Shader* shader, ColorBuffer* mainColorBuffer, ColorBuffer* screenSpaceShadowMap)
	{
		glDepthFunc(GL_ALWAYS);
		glDepthMask(GL_FALSE);
		glDisable(GL_CULL_FACE);
		shader->Use();
		shader->BindTexture("_MainTex", mainColorBuffer);
		shader->BindTexture("ScreenShadow", screenSpaceShadowMap);
		auto quad = Mesh::m_ScreenAlignedQuad;
		quad->Render(-1);
		glCheckError();
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glUseProgram(0);
	}



	RenderSystem::RenderSystem()
	{
		int w = Screen::GetWidth();
		int h = Screen::GetHeight();
		if (w <= 0)
			w = 1;
		if (h <= 0)
			h = 1;

		m_DepthPassRT = new RenderTarget();
		m_SceneDepth = DepthBuffer::Create(w, h);
		m_DepthPassRT->SetDepthBufferOnly(m_SceneDepth);
		m_RenderDepthShader = ShaderFromFile("RenderDepth.shader");

		m_CollectShadowsRT = new RenderTarget();
		m_ScreenSpaceShadowMap = ColorBuffer::Create(w, h, TextureFormat::R8);
		m_CollectShadowsShader = ShaderFromFile("CollectScreenSpaceShadow.shader");
		m_CollectShadowsRT->SetColorBufferOnly(m_ScreenSpaceShadowMap);

		m_CSMShader = ShaderFromFile("CascadedShadowMap.shader", true);

		m_MainRenderTarget = new RenderTarget();
		m_MainColorBuffer = ColorBuffer::Create(w, h);
		m_MainDepthBuffer = DepthBuffer::Create(w, h);
		m_MainRenderTarget->Set(m_MainColorBuffer, m_MainDepthBuffer);

		m_AddShadowShader = ShaderFromFile("PostProcessShadow.shader");
		m_AddShadowRT = new RenderTarget();
		m_AddShadowColorBuffer = ColorBuffer::Create(w, h);
		m_AddShadowRT->SetColorBufferOnly(m_AddShadowColorBuffer);

		Gizmos::Init();
	}


	void RenderDepthPass(Shader* shader, std::vector<RenderObject> const& renderObjects)
	{
		glFrontFace(GL_CW);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		shader->Use();
		for (auto&& ro : renderObjects)
		{
			auto& modelMat = ro.gameObject->GetTransform()->GetLocalToWorldMatrix();
			Pipeline::UpdatePerDrawUniforms(modelMat);
			shader->Use();
			ro.mesh->Render(-1);
		}
	}

	void RenderSystem::GetRenderObjects()
	{
		this->m_RenderObjects.clear();
		auto scene = SceneManager::GetActiveScene();
		auto mfs = scene->FindComponents<MeshFilter>();

//		auto& mrs = Object::FindObjectsOfType<MeshRenderer>();
		std::vector<GameObject*> intersection;
		for (auto mf : mfs)
		{
			auto go = ((MeshFilter*)mf)->GetGameObject();
			if (!go->IsActiveInHierarchy())
				continue;
//			auto mr = GetComponent<MeshRenderer>(go);
			auto mr = go->GetComponent<MeshRenderer>();
			if (mr != nullptr)
			{
				intersection.push_back(go);
			}
		}

		for (auto go : intersection)
		{
			auto mesh = go->GetComponent<MeshFilter>()->GetMesh();
			if (mesh != nullptr)
			{
				auto renderer = go->GetComponent<MeshRenderer>();
				auto material = renderer->GetMaterial();
				if (material == nullptr)
					material = Material::GetErrorMaterial();

				m_RenderObjects.emplace_back(go, renderer, mesh, material);
			}
		}
		glCheckError();

		auto skinnedMRs = scene->FindComponents<SkinnedMeshRenderer>();
		for (auto r : skinnedMRs)
		{
			auto mesh = r->GetSharedMesh();
			if (mesh == nullptr)
				continue;
			auto material = r->GetMaterial();
			if (material == nullptr)
				material = Material::GetErrorMaterial();

			auto go = r->GetGameObject();
			if (!go->IsActiveInHierarchy())
				continue;
			
			r->UpdateMatrixPalette();
			m_RenderObjects.emplace_back(go, r, mesh, material);
		}
	}


	void DrawSkeleton(const std::map<std::string, Transform*> const & skeleton)
	{
		glDisable(GL_DEPTH_TEST);
		for (auto&& p : skeleton)
		{
			auto t = p.second;
			auto parent = t->GetParent();
			if (parent != nullptr)
			{
				Gizmos::SetColor(Color::green);
				Gizmos::DrawLine(parent->GetPosition(), t->GetPosition());

				Gizmos::SetColor(Color::red);
				Gizmos::DrawWireSphere(t->GetPosition(), 0.02f);
			}
		}
		glEnable(GL_DEPTH_TEST);
	}

	void RenderSystem::Update()
	{
//		glCheckError();
//		glViewport(0, 0, Screen::GetWidth(), Screen::GetHeight());
//		glClearColor(0.2f, 0.3f, 0.3f, 1);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//
//		glFrontFace(GL_CW);
//		glEnable(GL_DEPTH_TEST);
//		glEnable(GL_CULL_FACE);
//		glCullFace(GL_BACK);

		auto scene = SceneManager::GetActiveScene();
		auto camera = scene->FindComponent<Camera>();
		auto light = scene->FindComponent<Light>();


		if (camera == nullptr || light == nullptr)
		{
			puts("camera or light is None");
			return;
		}
		Pipeline::BindLight(light);
		Pipeline::BindCamera(camera);


		this->GetRenderObjects();

		GLint old_framebuffer = 0;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &old_framebuffer);


		const int w = Screen::GetWidth();
		const int h = Screen::GetHeight();

		// Pre-Z
		m_SceneDepth->Resize(w, h);
		Pipeline::PushRenderTarget(m_DepthPassRT);
		glViewport(0, 0, w, h);
		RenderDepthPass(m_RenderDepthShader, m_RenderObjects);
		Pipeline::PopRenderTarget();


		// ShadowMap - CSM
		RenderShadowMap(m_CSMShader, camera, light, m_RenderObjects);

//		glFlush();

		// CollectShadowMap - ScreenSpaceShadowMap
		m_ScreenSpaceShadowMap->Resize(w, h);
		Pipeline::PushRenderTarget(m_CollectShadowsRT);
		glViewport(0, 0, w, h);
		glClearColor(0.f, 0.f, 0.f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		CollectShadows(m_CollectShadowsShader, light->m_shadowMap, m_SceneDepth);
		Pipeline::PopRenderTarget();

//		// test
//		glBindFramebuffer(GL_FRAMEBUFFER, old_framebuffer);
//		glCheckError();
//		glViewport(0, 0, w, h);
//		glClearColor(0.2f, 0.3f, 0.3f, 1);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//		DrawTexture(m_ScreenSpaceShadowMap);
////		ShowShadowMap(light->m_shadowMap);
//		glCheckError();

#if 1
//		glBindFramebuffer(GL_FRAMEBUFFER, old_framebuffer);

		m_MainDepthBuffer->Resize(w, h);
		m_MainColorBuffer->Resize(w, h);


		Pipeline::PushRenderTarget(m_MainRenderTarget);
		glViewport(0, 0, w, h);
		glClearColor(0.2f, 0.3f, 0.3f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glFrontFace(GL_CW);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		for (auto&& ro : m_RenderObjects)
		{
			auto& model = ro.gameObject->GetTransform()->GetLocalToWorldMatrix();
			Pipeline::UpdatePerDrawUniforms(model);
			Graphics::DrawMesh(ro.mesh, ro.material, -1);
		}


		Pipeline::PopRenderTarget();

		m_AddShadowColorBuffer->Resize(w, h);
		Pipeline::PushRenderTarget(m_AddShadowRT);
		AddShadow(m_AddShadowShader, m_MainColorBuffer, m_ScreenSpaceShadowMap);
		Pipeline::PopRenderTarget();


		glBindFramebuffer(GL_FRAMEBUFFER, old_framebuffer);
		DrawTexture(m_AddShadowColorBuffer);

		// blit depth buffer
//		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MainRenderTarget-);
		m_MainRenderTarget->AttachForRead();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, old_framebuffer);
		glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		// Skybox
		//if (false)
		{
			Scene* scene = SceneManager::GetActiveScene();
			Material* mat = scene->GetRenderSettings()->GetSkyboxMaterial();
			if (mat != nullptr)
			{
//				glCullFace(GL_FRONT);
				glDisable(GL_CULL_FACE);
				glDepthMask(GL_FALSE);
//				Matrix4x4 objectToWorld = Matrix4x4::TRS(camera->GetTransform()->GetPosition(), Quaternion(), Vector3(20, 20, 20));
				Matrix4x4 objectToWorld = Matrix4x4::Scale(camera->GetFarClipPlane());
				Graphics::DrawMesh(Mesh::m_SkyboxSphere, mat, -1, camera, objectToWorld, light);
				Graphics::DrawMesh(Mesh::m_SkyboxSphere, mat, -1, camera, objectToWorld, light);
				glDepthMask(GL_TRUE);
//				glCullFace(GL_BACK);
			}
		}

		auto animations = scene->FindComponents<Animation>();
		for (auto animation : animations)
		{
			DrawSkeleton(animation->m_skeleton);
		}


#endif

//		glDisable(GL_CULL_FACE);
//		ShowShadowMap();
	}
}

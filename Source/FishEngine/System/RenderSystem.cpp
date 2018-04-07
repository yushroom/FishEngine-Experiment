#include <FishEngine/System/RenderSystem.hpp>

#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/Component/Camera.hpp>
#include <FishEngine/Component/Light.hpp>
#include <FishEngine/Component/MeshFilter.hpp>
#include <FishEngine/Component/MeshRenderer.hpp>
#include <FishEngine/Render/Graphics.hpp>
#include <FishEngine/Render/GLEnvironment.hpp>
#include <FishEngine/Render/Shader.hpp>
#include <FishEngine/Scene.hpp>
#include <FishEngine/Render/Material.hpp>
#include <FishEngine/Render/Mesh.hpp>
#include <FishEngine/Render/Pipeline.hpp>

#include <FishEditor/Path.hpp>

#include <algorithm>

namespace FishEngine
{
#if 1

	void UpdateDepthMap(Camera* camera, Light* light, std::vector<MeshFilter*>& meshfilters)
	{
		glFrontFace(GL_CW);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		for (auto mf : meshfilters)
		{
			auto go = mf->GetGameObject();
			if (!go->IsActiveInHierarchy())
				continue;

			auto mesh = mf->GetMesh();
			if (mesh == nullptr)
				continue;

			auto renderer = go->GetComponent<Renderer>();
			if (renderer == nullptr || !renderer->GetEnabled())
			{
				continue;
			}

			auto modelMat = go->GetTransform()->GetLocalToWorldMatrix();
			Pipeline::UpdatePerDrawUniforms(modelMat);
			Graphics::DrawMesh(mesh, Material::GetDefaultMaterial());
		}
	}

	void RenderShadowMap(Camera* camera, Light* light, std::vector<MeshFilter*>& meshfilters)
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

#if 0
			light->m_projectMatrixForShadowMap[splitInex] = Matrix4x4::Ortho(min_p.x, max_p.x, min_p.y, max_p.y, z_near, z_far);
			light->m_viewMatrixForShadowMap[splitInex] = world_to_light;
#elif 0
			float scaleX = 2.0f / (max_p.x - min_p.x);
			float scaleY = 2.0f / (max_p.y - min_p.y);
			constexpr float scaleQuantizer = 64.0f;
			scaleX = 1.0f / std::ceilf(1.0f / scaleX * scaleQuantizer) * scaleQuantizer;
			scaleY = 1.0f / std::ceilf(1.0f / scaleY * scaleQuantizer) * scaleQuantizer;
			float offsetX = -0.5f * (max_p.x + min_p.x) * scaleX;
			float offsetY = -0.5f * (max_p.y + min_p.y) * scaleY;
			const float halfTextureSize = 0.5f * light->m_shadowMap->width();
			offsetX = std::ceilf(offsetX * halfTextureSize) / halfTextureSize;
			offsetY = std::ceilf(offsetY * halfTextureSize) / halfTextureSize;
			auto& forward = light_dir; // +z
			auto right = Vector3::Cross(Vector3(0, 1, 0), forward); // +x
			auto up = Vector3::Cross(forward, right); // +y
			eye_pos += right * offsetX + up * offsetY;
			split_centroid += right * offsetX + up * offsetY;
			//Gizmos::DrawWireSphere(eye_pos, 0.5f);
			world_to_light = Matrix4x4::LookAt(eye_pos, split_centroid, Vector3::up);
			light->m_projectMatrixForShadowMap[i] = Matrix4x4::Ortho(min_p.x, max_p.x, min_p.y, max_p.y, z_near, z_far);
			light->m_viewMatrixForShadowMap[i] = world_to_light;
#else
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
#endif

			light->m_cascadesSplitPlaneNear[splitInex] = split_near;
			light->m_cascadesSplitPlaneFar[splitInex] = split_far;
		}

//		auto shadow_map_material = Material::builtinMaterial("CascadedShadowMap");
		static Material* shadow_map_material = nullptr;
		if (shadow_map_material == nullptr)
		{
			shadow_map_material = new Material;
			auto str = FishEditor::ReadFileAsString("/Users/yushroom/program/FishEngine-Experiment/Assets/Shaders/CascadedShadowMap.shader");
			auto shader = Shader::FromString(str, true);
			shadow_map_material->SetShader(shader);
		}

		Pipeline::BindLight(light);

		auto shadowMap = light->m_shadowMap;
		Pipeline::PushRenderTarget(light->m_renderTarget);

		glViewport(0, 0, shadowMap->width(), shadowMap->height());
		glClear(GL_DEPTH_BUFFER_BIT);

		glFrontFace(GL_CW);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_CLAMP);

#if 1
		for (auto mf : meshfilters)
		{
			auto go = mf->GetGameObject();
			if (!go->IsActiveInHierarchy())
				continue;

			auto mesh = mf->GetMesh();
			if (mesh == nullptr)
				continue;

			auto renderer = go->GetComponent<Renderer>();
			if (renderer == nullptr ||
				!renderer->GetEnabled() ||
				renderer->GetCastShadows() == ShadowCastingMode::Off)
			{
				continue;
			}

			auto modelMat = go->GetTransform()->GetLocalToWorldMatrix();
			Pipeline::UpdatePerDrawUniforms(modelMat);
			Graphics::DrawMesh(mesh, shadow_map_material);
		}

#else
		for (auto& go : m_gameObjects)
		{
			bool is_skinned = false;

			if (!go->activeInHierarchy())
				continue;

			MeshPtr mesh;
			auto mesh_renderer = go->GetComponent<MeshRenderer>();
			if (mesh_renderer != nullptr)
			{
				if (mesh_renderer->shadowCastingMode() != ShadowCastingMode::Off)
				{
					auto meshFilter = go->GetComponent<MeshFilter>();
					if (meshFilter != nullptr)
					{
						mesh = meshFilter->mesh();
					}
				}
			}
			else
			{
				auto renderer = go->GetComponent<SkinnedMeshRenderer>();
				if (renderer != nullptr)
				{
					mesh = renderer->sharedMesh();
					if (renderer->m_avatar != nullptr)
					{
						shadow_map_material->EnableKeyword(ShaderKeyword::SkinnedAnimation);
						is_skinned = true;
						Pipeline::UpdateBonesUniforms(renderer->m_matrixPalette);
					}
				}
			}

			if (mesh != nullptr)
			{
				shader->BindUniformMat4("ObjectToWorld", go->transform()->localToWorldMatrix());
				shader->CheckStatus();
				mesh->Render();
				if (!is_skinned)
				{
					shadow_map_material->DisableKeyword(ShaderKeyword::SkinnedAnimation);
				}
			}
		}
#endif
		glDisable(GL_DEPTH_CLAMP);
		Pipeline::PopRenderTarget();
#undef DEBUG_SHADOW
	}


	void ShowShadowMap(Texture* shadowMap)
	{
		static Shader* shader = nullptr;
		if (shader == nullptr)
		{
			auto str = FishEditor::ReadFileAsString("/Users/yushroom/program/FishEngine-Experiment/Assets/Shaders/DisplayCSM.shader");
			shader = Shader::FromString(str);
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
		static Shader* shader = nullptr;
		if (shader == nullptr)
		{
			auto str = FishEditor::ReadFileAsString("/Users/yushroom/program/FishEngine-Experiment/Assets/Shaders/DrawQuad.shader");
			shader = Shader::FromString(str);
		}
		glDisable(GL_CULL_FACE);
		shader->Use();
		shader->BindTexture("_MainTex", shadowMap);
		auto quad = Mesh::m_Quad;
		quad->Render(-1);
		glCheckError();
	}

	void CollectShadows(Camera* camera, Light* light, DepthBuffer* sceneDepth)
	{
		static Shader* shader = nullptr;
		if (shader == nullptr)
		{
			auto str = FishEditor::ReadFileAsString("/Users/yushroom/program/FishEngine-Experiment/Assets/Shaders/CollectScreenSpaceShadow.shader");
			shader = Shader::FromString(str);
		}
		glDepthFunc(GL_ALWAYS);
		glDepthMask(GL_FALSE);
		glDisable(GL_CULL_FACE);
		shader->Use();
//		shader->BindTexture("_MainTex", sceneDepth);
		shader->BindTexture("SceneDepthTexture", sceneDepth);
		shader->BindTexture("CascadedShadowMap", light->m_shadowMap);
//		glBindTexture(GL_TEXTURE_2D_ARRAY, light->m_shadowMap->GetNativeTexturePtr());
//		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
//		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
//		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		light->m_shadowMap->setFilterMode(FilterMode::Bilinear);
		float shadowMapSize = static_cast<float>( light->m_shadowMap->width() );
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

#endif

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

		auto mfs = scene->FindComponents<MeshFilter>();

		GLint framebuffer = 0;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &framebuffer);

		static RenderTarget rt;
		const int w = Screen::GetWidth();
		const int h = Screen::GetHeight();
		static auto color = ColorBuffer::Create(w, h);
		static auto sceneDepth = DepthBuffer::Create(w, h);
		if (rt.GetColorBuffer() == nullptr)
			rt.Set(color, sceneDepth);
		color->Resize(w, h);
		sceneDepth->Resize(w, h);
		Pipeline::PushRenderTarget(&rt);
		glViewport(0, 0, w, h);
		UpdateDepthMap(camera, light, mfs);
		Pipeline::PopRenderTarget();


		RenderShadowMap(camera, light, mfs);
//		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

//		Pipeline::BindCamera(camera);
//		Pipeline::BindLight(light);

//		glCheckError();
//		glViewport(0, 0, Screen::GetWidth(), Screen::GetHeight());
//		glClearColor(0.2f, 0.3f, 0.3f, 1);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

//		glFrontFace(GL_CW);
//		glEnable(GL_DEPTH_TEST);
//		glEnable(GL_CULL_FACE);
//		glCullFace(GL_BACK);

//		glDisable(GL_CULL_FACE);
//		ShowShadowMap(light->m_shadowMap);
//		ShowShadowMap(depth);

//		glFlush();

		static RenderTarget rt2;
		static auto color2 = ColorBuffer::Create(w, h, TextureFormat::R8);
//		static auto depth2 = DepthBuffer::Create(w, h);
		if (rt2.GetColorBuffer() == nullptr)
		{
//			rt2.Set(color2, depth2);
			rt2.SetColorBufferOnly(color2);
		}
		color2->Resize(w, h);
//		depth2->Resize(Screen::GetWidth(), Screen::GetHeight());
		Pipeline::PushRenderTarget(&rt2);
		glViewport(0, 0, w, h);
		glClearColor(0.f, 0.f, 0.f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		CollectShadows(camera, light, sceneDepth);
		Pipeline::PopRenderTarget();

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glCheckError();
		glViewport(0, 0, w, h);
		glClearColor(0.2f, 0.3f, 0.3f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		DrawTexture(color2);
//		ShowShadowMap(light->m_shadowMap);
		glCheckError();

#if 0
		glViewport(0, 0, Screen::GetWidth(), Screen::GetHeight());
		glClearColor(0.2f, 0.3f, 0.3f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glFrontFace(GL_CW);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

//		auto& mrs = Object::FindObjectsOfType<MeshRenderer>();
		std::vector<GameObject*> intersection;
		for (auto mf : mfs)
		{
			auto go = ((MeshRenderer*)mf)->GetGameObject();
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
				auto material = go->GetComponent<MeshRenderer>()->GetMaterial();
				if (material == nullptr)
					material = Material::GetErrorMaterial();

				auto& model = go->GetTransform()->GetLocalToWorldMatrix();
				Pipeline::UpdatePerDrawUniforms(model);
//				Graphics::DrawMesh(mesh, material, -1, camera, model, light);
				Graphics::DrawMesh(mesh, material, -1);
			}
		}
		glCheckError();


		// Skybox
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
				glDepthMask(GL_TRUE);
//				glCullFace(GL_BACK);
			}
		}
#endif

//		glDisable(GL_CULL_FACE);
//		ShowShadowMap();
	}
}

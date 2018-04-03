#include <FishEngine/System/RenderSystem.hpp>

#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/Component/Camera.hpp>
#include <FishEngine/Component/Light.hpp>
#include <FishEngine/Component/MeshFilter.hpp>
#include <FishEngine/Component/MeshRenderer.hpp>

#include <FishEngine/Render/Graphics.hpp>
#include <FishEngine/Render/GLEnvironment.hpp>
#include <FishEngine/Scene.hpp>

#include <FishEngine/Render/Material.hpp>
#include <FishEngine/Render/Mesh.hpp>

namespace FishEngine
{
	void RenderSystem::Update()
	{
		glCheckError();
		glViewport(0, 0, Screen::GetWidth(), Screen::GetHeight());
		glClearColor(0.2f, 0.3f, 0.3f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		glFrontFace(GL_CW);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		auto scene = SceneManager::GetActiveScene();
		auto camera = scene->FindComponent<Camera>();
		auto light = scene->FindComponent<Light>();


		if (camera == nullptr || light == nullptr)
		{
			puts("camera or light is None");
			return;
		}
		
		auto mfs = scene->FindComponents<MeshFilter>();
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
				glCheckError();
				Graphics::DrawMesh(mesh, material, -1, camera, model, light);
				glCheckError();
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
	}
}

#include <FishEngine/System/RenderSystem.hpp>

#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/Component/Camera.hpp>
#include <FishEngine/Component/Light.hpp>
#include <FishEngine/Component/MeshFilter.hpp>
#include <FishEngine/Component/MeshRenderer.hpp>

#include <FishEngine/Render/Graphics.hpp>
#include <FishEngine/Render/GLEnvironment.hpp>

#include <FishEngine/Render/Material.hpp>

//#include <typeindex>

namespace FishEngine
{
//	template<class T>
//	T* GetComponent(GameObject* go)
//	{
//		for (auto c : go->m_components)
//		{
//			if (typeid(*c).hash_code() == typeid(T).hash_code())
//			{
//				return dynamic_cast<T*>(c);
//			}
//		}
//		return nullptr;
//	}
	
	void RenderSystem::Update()
	{
		glViewport(0, 0, Screen::width(), Screen::height());
		glClearColor(0.2f, 0.3f, 0.3f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		glFrontFace(GL_CW);
		glEnable(GL_DEPTH_TEST);
		
		
		auto camera = Object::FindObjectOfType<Camera>();
		auto light = Object::FindObjectOfType<Light>();
		if (camera == nullptr || light == nullptr)
		{
			puts("camera or light is None");
			return;
		}
		
		auto& mfs = Object::FindObjectsOfType<MeshFilter>();
//		auto& mrs = Object::FindObjectsOfType<MeshRenderer>();
		std::vector<GameObject*> intersection;
		for (auto mf : mfs)
		{
			auto go = ((MeshRenderer*)mf)->gameObject();
//			auto mr = GetComponent<MeshRenderer>(go);
			auto mr = go->GetComponent<MeshRenderer>();
			if (mr != nullptr)
			{
				intersection.push_back(go);
			}
		}
		for (auto go : intersection)
		{
			auto mesh = go->GetComponent<MeshFilter>()->m_mesh;
			if (mesh != nullptr)
			{
				auto material = go->GetComponent<MeshRenderer>()->m_material;
				if (material == nullptr)
					material = Material::GetErrorMaterial();
				auto& model = go->m_transform->localToWorldMatrix();
				Graphics::DrawMesh(mesh, material, -1, camera, model, light);
			}
		}
	}
	
	glCheckError();
}

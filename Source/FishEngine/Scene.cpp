#include <FishEngine/Scene.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>

#include <FishEngine/CloneObject.hpp>
#include <FishEngine/Serialization/CloneArchive.hpp>
#include <algorithm>

namespace FishEngine
{
	Scene* SceneManager::s_ActiveScene = nullptr;

	int Scene::s_Counter = 0;

	Scene::Scene()
	{
		s_Counter++;
		m_Handle = s_Counter;
		SceneManager::s_HandleToScene[m_Handle] = this;
	}

	Scene::~Scene()
	{
		delete m_RenderSettings;
		this->Clean();
		auto& m = SceneManager::s_HandleToScene;
		m.erase(m.find(m_Handle));
		if (SceneManager::s_ActiveScene == this)
			SceneManager::SetActiveScene(nullptr);
	}
	
//	std::vector<GameObject*> Scene::GetRootGameObjects()
//	{
//		std::vector<GameObject*> result;
//		for (auto t : m_rootTransforms)
//		{
//			result.push_back(t->gameObject());
//		}
//		return result;
//	}
	
	void Scene::AddRootTransform(Transform* t)
	{
		if (m_Cleaning)
			return;
		m_RootTransforms.push_back(t);
//		t->m_RootOrder = m_RootTransforms.size() - 1;
	}
	
	void Scene::RemoveRootTransform(Transform* t)
	{
		auto pos = std::find(m_RootTransforms.begin(), m_RootTransforms.end(), t);
		assert(pos != m_RootTransforms.end());
		m_RootTransforms.erase(pos);
	}
	
//	void CleanRecursively(Transform* t)
//	{
//		if (!t->GetChildren().empty())
//		{
//			for (auto c : t->GetChildren())
//			{
//				CleanRecursively(c);
//			}
//		}
//		delete t->GetGameObject();
//	}
	
	void Scene::Clean()
	{
//		for (auto t : m_rootTransforms)
//		{
//			delete t;
//		}
		//m_rootTransforms.clear();
//		for (auto t : m_rootTransforms)
		m_Cleaning = true;
		for (int i = (int)m_RootTransforms.size()-1; i >= 0; --i)
		{
			auto t = m_RootTransforms[i];
			delete t->GetGameObject();
		}
		m_RootTransforms.clear();
		m_Cleaning = false;
	}
	
	Scene* Scene::Clone()
	{
		std::map<Object*, Object*> memo;
		return this->CloneWithMemo(memo);
	}

	Scene* Scene::CloneWithMemo(std::map<Object*, Object*>& memo)
	{
		Scene* old = SceneManager::GetActiveScene();
		Scene* cloned = SceneManager::CreateScene(this->m_Name + "-cloned");
		SceneManager::SetActiveScene(cloned);
		cloned->m_Path = this->m_Path;
		cloned->m_RootTransforms.reserve(this->m_RootTransforms.size());

		CollectObjectsArchive archive;
		std::vector<Object*> objects;
//		archive.Collect(m_RenderSettings);
		for (auto t : m_RootTransforms)
		{
			auto go = t->GetGameObject();
			objects.push_back(go);
			archive.Collect(go);
		}

		for (auto o : archive.m_Objects)
		{
			if (o->Is<GameObject>())
			{
				auto go = o->As<GameObject>();
				if (go->GetPrefabParentObject() != nullptr)
					memo[go->GetPrefabParentObject()] = nullptr;
				if (go->GetPrefabParentObject() != nullptr)
					memo[go->GetPrefabParentObject()] = nullptr;
			}
			else if (o->Is<Component>())
			{
				auto comp = o->As<Component>();
				if (comp->GetPrefabParentObject() != nullptr)
					memo[comp->GetPrefabParentObject()] = nullptr;
			}
//			objects.push_back(o);
		}

//		std::map<Object*, Object*> memo;
		auto clonedObjects = CloneObjects(objects, memo);

		for (auto o : clonedObjects)
		{
			cloned->m_RootTransforms.push_back(o->As<GameObject>()->GetTransform());
		}
//		cloned->m_RenderSettings = memo[this->m_RenderSettings]->As<RenderSettings>();
		cloned->m_RenderSettings = CloneObject(this->m_RenderSettings)->As<RenderSettings>();
//
//		for (auto t : m_RootTransforms)
//		{
//			cloned->m_RootTransforms.push_back(memo[t]->As<Transform>());
//		}

		SceneManager::SetActiveScene(old);
		return cloned;
	}

	std::map<int, Scene*> SceneManager::s_HandleToScene;
}

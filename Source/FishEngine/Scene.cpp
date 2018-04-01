#include <FishEngine/Scene.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>

#include <FishEngine/CloneObject.hpp>


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
		m_RootTransforms.push_back(t);
		t->m_RootOrder = m_RootTransforms.size() - 1;
	}
	
	void Scene::RemoveRootTransform(Transform* t)
	{
//		int oldsize = m_rootTransforms.size();
//		std::remove(m_rootTransforms.begin(), m_rootTransforms.end(), t);
//		assert(oldsize-1 == m_rootTransforms.size());

		int oldsize = (int)m_RootTransforms.size();
		for (auto it = m_RootTransforms.begin(); it != m_RootTransforms.end(); ++it)
		{
			if (t == *it)
			{
				m_RootTransforms.erase(it);
				break;
			}
		}
		assert(oldsize-1 == m_RootTransforms.size());
		
//		auto it = m_rootTransforms.begin();
//		std::advance(it, t->m_RootOrder);
//		assert(*it == t);
//		m_rootTransforms.erase(it);
		
//		for (int i = 0; i < m_rootTransforms.size(); ++i)
//		{
//			m_rootTransforms[i]->m_RootOrder = i;
//		}
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
		for (int i = (int)m_RootTransforms.size()-1; i >= 0; --i)
		{
			auto t = m_RootTransforms[i];
			delete t->GetGameObject();
		}
		m_RootTransforms.clear();
	}
	
	Scene* Scene::Clone()
	{
		Scene* old = SceneManager::GetActiveScene();
		Scene* cloned = new Scene;
		SceneManager::SetActiveScene(cloned);
		cloned->m_Name = this->m_Name + "-cloned";
		cloned->m_Path = this->m_Path;
		cloned->m_RootTransforms.reserve(this->m_RootTransforms.size());

		for (int i = 0; i < m_RootTransforms.size(); ++i)
		{
			auto go = m_RootTransforms[i]->GetGameObject();
			auto cloned_go = CloneObject(go)->As<GameObject>();
			cloned->AddRootTransform(cloned_go->GetTransform());
		}

		SceneManager::SetActiveScene(old);
		return cloned;
	}

	std::map<int, Scene*> SceneManager::s_HandleToScene;
}

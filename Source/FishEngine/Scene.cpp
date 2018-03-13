#include <FishEngine/Scene.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>

namespace FishEngine
{
	Scene* SceneManager::s_activeScene = nullptr;

	int Scene::s_counter = 0;

	Scene::Scene()
	{
		s_counter++;
		m_Handle = s_counter;
		SceneManager::s_handleToScene[m_Handle] = this;
	}

	Scene::~Scene()
	{
		auto& m = SceneManager::s_handleToScene;
		m.erase(m.find(m_Handle));
		if (SceneManager::s_activeScene == this)
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
		m_rootTransforms.push_back(t);
//		t->m_RootOrder = m_rootTransforms.size() - 1;
	}
	
	void Scene::RemoveRootTransform(Transform* t)
	{
//		int oldsize = m_rootTransforms.size();
//		std::remove(m_rootTransforms.begin(), m_rootTransforms.end(), t);
//		assert(oldsize-1 == m_rootTransforms.size());

		int oldsize = (int)m_rootTransforms.size();
		for (auto it = m_rootTransforms.begin(); it != m_rootTransforms.end(); ++it)
		{
			if (t == *it)
			{
				m_rootTransforms.erase(it);
				break;
			}
		}
		assert(oldsize-1 == m_rootTransforms.size());
		
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
		for (int i = (int)m_rootTransforms.size()-1; i >= 0; --i)
		{
			auto t = m_rootTransforms[i];
			delete t->GetGameObject();
		}
		m_rootTransforms.clear();
	}
	
	Scene* Scene::Clone()
	{
		Scene* cloned = new Scene;
		SceneManager::SetActiveScene(cloned);
		cloned->m_name = this->m_name + "-cloned";
		cloned->m_path = this->m_path;
		cloned->m_rootTransforms.reserve(this->m_rootTransforms.size());
		
		for (int i = 0; i < m_rootTransforms.size(); ++i)
		{
			auto go = m_rootTransforms[i]->GetGameObject();
			go->Clone();
		}
		
		return cloned;
	}

	std::map<int, Scene*> SceneManager::s_handleToScene;
}

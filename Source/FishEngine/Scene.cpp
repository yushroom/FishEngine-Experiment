#include <FishEngine/Scene.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>

namespace FishEngine
{
	Scene* SceneManager::s_activeScene = nullptr;
	
//	std::vector<GameObject*> Scene::GetRootGameObjects()
//	{
//		std::vector<GameObject*> result;
//		for (auto t : m_rootTransforms)
//		{
//			result.push_back(t->gameObject());
//		}
//		return result;
//	}
	
	void Scene::AddTransform(Transform* t)
	{
		m_rootTransforms.push_back(t);
		t->m_RootOrder = m_rootTransforms.size() - 1;
	}
	
	void Scene::RemoveTransform(Transform* t)
	{
		auto it = m_rootTransforms.begin();
		std::advance(it, t->m_RootOrder);
		assert(*it == t);
		m_rootTransforms.erase(it);
		
		for (int i = 0; i < m_rootTransforms.size(); ++i)
		{
			m_rootTransforms[i]->m_RootOrder = i;
		}
	}
}

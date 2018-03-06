#pragma once
#include "Object.hpp"
#include <vector>
#include <list>

namespace FishEngine
{
	class GameObject;
	class Transform;
	
	class Scene
	{
	public:
		
		Scene() = default;
		Scene(Scene&) = delete;
		Scene& operator=(Scene&) = delete;
		
		int GetRootCount()
		{
			return m_rootTransforms.size();
		}
		
		const std::vector<Transform*>& GetRootTransforms()
		{
			return m_rootTransforms;
		}
//		std::vector<GameObject*> GetRootGameObjects();
		
		void AddTransform(Transform* t);
		
		void RemoveTransform(Transform* t);
		
	private:
		friend class Transform;
		std::vector<Transform*> m_rootTransforms;
	};
	
	
	class SceneManager
	{
	public:
		SceneManager() = delete;
		
		static Scene* GetActiveScene()
		{
//			assert(s_activeScene != nullptr);
			if (s_activeScene == nullptr)
			{
				// TODO: delete
				s_activeScene = new Scene();
			}
			return s_activeScene;
		}
		
	private:
		static std::list<Scene*> s_scenes;
		static Scene* s_activeScene;
	};
}

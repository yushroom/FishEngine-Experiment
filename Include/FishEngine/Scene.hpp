#pragma once
#include "Object.hpp"
#include <vector>
#include <list>
#include "GameObject.hpp"
#include "Transform.hpp"

namespace FishEngine
{
//	class GameObject;
//	class Transform;
	
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
		
		void Clean();
		
		Scene* Clone();
		
		template<class T>
		T* FindComponent()
		{
//			for (auto t : m_rootTransforms)
//			{
//				auto c = t->GetGameObject()->template GetComponentInChildren<T>();
//				if (c != nullptr)
//					return c;
//			}
//			return nullptr;
			auto& all = Object::FindObjectsOfType<T>();
			for (auto c : all)
			{
				auto cc = (T*)c;
				if (cc->GetGameObject()->GetScene() == this)
					return cc;
			}
			return nullptr;
		}
		
		template<class T>
		std::vector<T*> FindComponents()
		{
			std::vector<T*> components;
			for (auto t : m_rootTransforms)
			{
				t->GetGameObject()->template GetComponentsInChildren<T>(components);
			}
			return components;
		}
	
	private:
		void AddRootTransform(Transform* t);
		void RemoveRootTransform(Transform* t);
		
	private:
		friend class Transform;
		friend class GameObject;
		std::vector<Transform*> m_rootTransforms;
		
		std::string m_name;
		
		// relative path of the scene. Like: "Assets/MyScenes/MyScene.unity".
		std::string m_path;
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
		friend class Scene;
		static std::list<Scene*> s_scenes;
		static Scene* s_activeScene;
	};
}

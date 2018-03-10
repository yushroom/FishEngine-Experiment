#pragma once
#include "Object.hpp"
#include <vector>
#include <list>
#include <map>
#include "GameObject.hpp"
#include "Transform.hpp"

namespace FishEngine
{
//	class GameObject;
//	class Transform;
	
	class Scene
	{
	public:
		
		Scene();
		~Scene();

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
		friend class SceneManager;

		std::vector<Transform*> m_rootTransforms;
		
		int m_Handle = 0;	// 0 is invalid
		std::string m_name;
		
		// relative path of the scene. Like: "Assets/MyScenes/MyScene.unity".
		std::string m_path;

		static int s_counter;
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

		static void SetActiveScene(Scene* scene)
		{
			s_activeScene = scene;
		}

		static Scene* GetSceneByHandle(int handle)
		{
			return s_handeToScene[handle];
		}

		static void StaticClean()
		{
			std::vector<Scene*> scenes;
			for (auto&& p : s_handeToScene)
			{
				auto scene = p.second;
				scene->Clean();
				scenes.push_back(scene);
				// ~Scene will modified s_handleToScene
				//delete scene;	
			}
			for (auto s : scenes)
			{
				delete s;
			}
			s_handeToScene.clear();
		}
		
	private:
		friend class Scene;
		static std::map<int, Scene*> s_handeToScene;
		//static std::list<Scene*> s_scenes;
		static Scene* s_activeScene;
	};
}

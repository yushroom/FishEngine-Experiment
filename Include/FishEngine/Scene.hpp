#pragma once
#include "Object.hpp"
#include <vector>
#include <list>
#include <map>
#include "GameObject.hpp"
#include "Transform.hpp"

#include <cassert>

namespace FishEditor
{
	class DefaultImporter;
}

namespace FishEngine
{
//	class GameObject;
//	class Transform;
	
	class Scene
	{
	public:
		
//		Scene();
		~Scene();

		Scene(Scene&) = delete;
		Scene& operator=(Scene&) = delete;
		
		int GetRootCount()
		{
			return static_cast<int>( m_RootTransforms.size() );
		}
		
		const std::vector<Transform*>& GetRootTransforms()
		{
			return m_RootTransforms;
		}
		
		void Clean();
		
		Scene* Clone();
		Scene* CloneWithMemo(std::map<Object*, Object*>& memo);
		
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
			for (auto t : m_RootTransforms)
			{
				t->GetGameObject()->template GetComponentsInChildren<T>(components);
			}
			return components;
		}
		
		int GetHandle() const
		{
			return m_Handle;
		}
		
		const std::string& GetName() const
		{
			return m_Name;
		}
	
	private:
		Scene();
		
		void AddRootTransform(Transform* t);
		void RemoveRootTransform(Transform* t);
		
	private:
		friend class Transform;
		friend class GameObject;
		friend class SceneManager;
		friend class FishEditor::DefaultImporter;

		std::vector<Transform*> m_RootTransforms;
		
		int m_Handle = 0;	// 0 is invalid
		std::string m_Name;
		
		// relative path of the scene. Like: "Assets/MyScenes/MyScene.unity".
		std::string m_Path;

		static int s_Counter;
	};
	
	
	class SceneManager
	{
	public:
		SceneManager() = delete;
		
		static Scene* CreateScene(const std::string& name)
		{
			auto scene = new Scene();
			scene->m_Name = name;
			return scene;
		}
		
		static Scene* GetActiveScene()
		{
			assert(s_ActiveScene != nullptr);
//			if (s_activeScene == nullptr)
//			{
//				// TODO: delete
//				s_activeScene = new Scene();
//			}
			return s_ActiveScene;
		}

		static void SetActiveScene(Scene* scene)
		{
			s_ActiveScene = scene;
		}

		static Scene* GetSceneByHandle(int handle)
		{
			return s_HandleToScene[handle];
		}

		static void StaticClean()
		{
			std::vector<Scene*> scenes;
			for (auto&& p : s_HandleToScene)
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
			s_HandleToScene.clear();
		}
		
	private:
		friend class Scene;
		static std::map<int, Scene*> s_HandleToScene;
		//static std::list<Scene*> s_scenes;
		static Scene* s_ActiveScene;
	};
}

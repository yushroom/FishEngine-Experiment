#pragma once

#include "FishEngine.hpp"
#include "Object.hpp"
#include "Scene.hpp"

#include <list>
#include <vector>

namespace FishEngine
{
    class Component;
    class Transform;
	class RectTransform;

    class GameObject : public Object
    {
    public:
//		static constexpr int ClassID = 1;
		enum {ClassID = 1};
		
//	protected:
        GameObject(const std::string& name = "GameObject");
	
	public:
		~GameObject();
		
		// for python
		static GameObject* Create(const std::string& name)
		{
			return new GameObject(name);
		}
		
        // for python
		// create GameObject and bind transform
//		static GameObject* CreateWithTransform(const std::string& name = "GameObject");
		
		// for python
		// special version for RectTransform
//		static GameObject* CreateWithRectTransform(const std::string& name = "GameObject");

		void AddComponent(Component* comp);
//		void AddScript(Script* compo);
		
//		void AddRectTransform(RectTransform* t);
//		void RemoveRectTransform();

//        static std::list<GameObject*> s_gameObjects;

        Transform* m_transform = nullptr;
        std::list<Component*> m_components;
//        std::list<Script*> m_scripts;
		
		Scene* GetScene() const
		{
			return m_scene;
		}

    protected:
		
		Scene* m_scene = nullptr;
    };
}

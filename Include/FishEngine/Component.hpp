#pragma once

#include "Object.hpp"

namespace FishEngine
{
    class GameObject;
	class Transform;

    class Component : public Object
    {
    public:
		Component()
		{
//			LOGF;
		}
		
        virtual ~Component()
		{
//			LOGF;
		}
		
		virtual bool IsScript() const
		{
			return false;
		}
		
		Transform* transform() const;
		GameObject* gameObject() const
		{
			return m_gameObject;
		}

    // protected:
        GameObject* m_gameObject = nullptr;
    };
}

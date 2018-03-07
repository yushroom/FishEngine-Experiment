#pragma once

#include "Object.hpp"

namespace FishEngine
{
    class GameObject;
	class Transform;

    class Component : public Object
    {
    public:
		enum {ClassID = 2};
		Component(int classID) : Object(classID)
		{
//			LOGF;
		}
		
        virtual ~Component()
		{
//			LOGF;
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

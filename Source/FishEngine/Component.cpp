#include <FishEngine/Component.hpp>

#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>

namespace FishEngine
{
	Transform* Component::GetTransform() const
	{
		return m_GameObject->GetTransform();
	}

//	void Component::SetGameObject(GameObject* gameoObject)
//	{
//		assert(gameoObject != nullptr);
//		gameoObject->AddComponent(this);
//		m_GameObject = gameoObject;
//	}
}

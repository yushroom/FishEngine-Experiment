#include <FishEngine/Component.hpp>

#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>

namespace FishEngine
{
	Transform* Component::transform() const
	{
		return m_gameObject->m_transform;
	}
}

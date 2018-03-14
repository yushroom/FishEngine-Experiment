#include <FishEngine/Component.hpp>

#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>

namespace FishEngine
{
	Transform* Component::GetTransform() const
	{
		return m_GameObject->GetTransform();
	}
}

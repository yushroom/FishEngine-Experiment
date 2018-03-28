#include <FishEngine/Component/MeshRenderer.hpp>

namespace FishEngine
{
	MeshRenderer* MeshRenderer::Clone() const
	{
		auto cloned = new MeshRenderer();
		cloned->m_Material = m_Material;
		cloned->m_Name = cloned->m_Name;
		return cloned;
	}
}
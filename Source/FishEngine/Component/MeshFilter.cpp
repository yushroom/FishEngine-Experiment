#include <FishEngine/Component/MeshFilter.hpp>

namespace FishEngine
{
	MeshFilter* MeshFilter::Clone() const
	{
		auto cloned = new MeshFilter();
		cloned->m_mesh = m_mesh;
		cloned->m_Name = m_Name;
		return cloned;
	}
}
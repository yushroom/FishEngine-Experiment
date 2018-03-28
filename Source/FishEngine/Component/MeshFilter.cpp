#include <FishEngine/Component/MeshFilter.hpp>

namespace FishEngine
{
	MeshFilter* MeshFilter::Clone() const
	{
		auto cloned = new MeshFilter();
		cloned->m_Mesh = m_Mesh;
		cloned->m_Name = m_Name;
		return cloned;
	}
}
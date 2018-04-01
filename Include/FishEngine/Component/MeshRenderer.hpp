#pragma once

#include <vector>
#include "../Component.hpp"

namespace FishEngine
{
	class Material;
	
	class MeshRenderer : public Component
	{
	public:
		DeclareObject(MeshRenderer, 23);

		
		MeshRenderer() : Component(MeshRenderer::ClassID, ClassName)
		{
		}
		
		~MeshRenderer()
		{
		}
		
		Material* GetMaterial() const
		{
			if (m_Materials.empty())
				return nullptr;
			return m_Materials[0];
		}
		
		void SetMaterial(Material* mat)
		{
			if (m_Materials.empty())
				m_Materials.push_back(mat);
			else
				m_Materials[0] = mat;
		}

	private:
		std::vector<Material*> m_Materials;
	};
}

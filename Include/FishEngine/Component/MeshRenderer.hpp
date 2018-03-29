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
		
		Material* GetMaterial() const { return m_Materials[0]; }
		void SetMaterial(Material* mat) { m_Materials[0] = mat; }
		
		virtual MeshRenderer* Clone() const override;

	private:
		std::vector<Material*> m_Materials;
	};
}

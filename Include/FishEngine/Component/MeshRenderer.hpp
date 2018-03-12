#pragma once

#include "../Component.hpp"

namespace FishEngine
{
	class Material;
	
	class MeshRenderer : public Component
	{
	public:
		enum {ClassID = 23};
		
		MeshRenderer() : Component(MeshRenderer::ClassID)
		{
		}
		
		~MeshRenderer()
		{
		}
		
		Material* GetMaterial() const { return m_material; }
		void SetMaterial(Material* mat) { m_material = mat; }
		
		virtual MeshRenderer* Clone() const override;

	private:
		Material* m_material = nullptr;
	};
}

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
		
		Material* GetMaterial() const { return m_Material; }
		void SetMaterial(Material* mat) { m_Material = mat; }
		
		virtual MeshRenderer* Clone() const override;
		DeclareSerializeFunc;

	private:
		Material* m_Material = nullptr;
	};
}

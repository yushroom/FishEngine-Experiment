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
		
//	private:
		Material* m_material = nullptr;
	};
}

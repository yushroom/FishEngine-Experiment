#pragma once

#include <vector>
#include "Renderer.hpp"

namespace FishEngine
{
	class MeshRenderer : public Renderer
	{
	public:
		DeclareObject(MeshRenderer, 23);
		
		MeshRenderer() : Renderer(ClassID, ClassName)
		{
		}
		
		~MeshRenderer()
		{
		}
	};
}

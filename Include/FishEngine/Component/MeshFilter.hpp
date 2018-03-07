#pragma once

#include "../Component.hpp"

namespace FishEngine
{
	class Mesh;
	
	class MeshFilter : public Component
	{
	public:
		enum {ClassID = 33};
		
		MeshFilter() : Component(MeshFilter::ClassID)
		{
		}
		
		~MeshFilter()
		{
		}
		
//	private:
		Mesh* m_mesh = nullptr;
	};
}

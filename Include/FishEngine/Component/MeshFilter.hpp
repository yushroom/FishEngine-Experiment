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

		Mesh* GetMesh() const
		{
			return m_mesh;
		}

		virtual MeshFilter* Clone() const override;
		
//	private:
		Mesh* m_mesh = nullptr;
	};
}

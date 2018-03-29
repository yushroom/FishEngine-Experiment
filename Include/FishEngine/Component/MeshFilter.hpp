#pragma once

#include "../Component.hpp"

namespace FishEngine
{
	class Mesh;
	
	class MeshFilter : public Component
	{
	public:
		DeclareObject(MeshFilter, 33);
		
		MeshFilter() : Component(MeshFilter::ClassID, ClassName)
		{
		}
		
		~MeshFilter()
		{
		}

		Mesh* GetMesh() const { return m_Mesh; }
		void SetMesh(Mesh* mesh) { m_Mesh = mesh; }

		virtual MeshFilter* Clone() const override;
		
	private:
		Mesh* m_Mesh = nullptr;
	};
}

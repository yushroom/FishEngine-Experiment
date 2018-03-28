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

		Mesh* GetMesh() const { return m_Mesh; }
		void SetMesh(Mesh* mesh) { m_Mesh = mesh; }

		virtual MeshFilter* Clone() const override;
		DeclareSerializeFunc;
		
	private:
		Mesh* m_Mesh = nullptr;
	};
}

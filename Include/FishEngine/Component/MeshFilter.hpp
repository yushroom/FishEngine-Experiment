#pragma once

#include "../Component.hpp"

namespace FishEngine
{
	class Mesh;
	
	class MeshFilter : public Component
	{
	public:
		enum {ClassID = 33};
		
		MeshFilter()
		{
			Object::s_objects[ClassID].insert(this);
		}
		
		~MeshFilter()
		{
			Object::s_objects[ClassID].erase(this);
		}
		
//	private:
		Mesh* m_mesh = nullptr;
	};
}

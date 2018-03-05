#pragma once

#include "../Component.hpp"

namespace FishEngine
{
	class Material;
	
	class MeshRenderer : public Component
	{
	public:
		enum {ClassID = 23};
		
		MeshRenderer()
		{
			Object::s_objects[ClassID].insert(this);
		}
		
		~MeshRenderer()
		{
			Object::s_objects[ClassID].erase(this);
		}
		
//	private:
		Material* m_material = nullptr;
	};
}

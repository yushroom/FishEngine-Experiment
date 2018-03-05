#pragma once

#include <cstdio>
#include "../Object.hpp"

namespace FishEngine
{
	class Shader;
	
	class Material : public Object
	{
	public:
		
		enum {ClassID = 21};
		
		Material()
		{
//			LOGF;
			Object::s_objects[ClassID].insert(this);
		}
		Material(const Material&) = delete;
		const Material& operator=(const Material&) = delete;
		
		~Material()
		{
//			LOGF;
			Object::s_objects[ClassID].erase(this);
		}
		
		// for python
		static Material* Create()
		{
			return new Material();
		}
		
		Shader* GetShader() const
		{
			return m_shader;
		}
		
		void SetShader(Shader* shader)
		{
			m_shader = shader;
		}
		
		static void StaticInit();
		static void StaticClean();
		
		static Material* GetErrorMaterial()
		{
			return s_errorMaterial;
		}
		
	protected:
		Shader* m_shader = nullptr;
		
		static Material* s_errorMaterial;
		static Material* s_defaultMaterial;
	};
}

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
		
		Material() : Object(Material::ClassID)
		{
//			LOGF;
		}
		Material(const Material&) = delete;
		const Material& operator=(const Material&) = delete;
		
		~Material()
		{
//			LOGF;
		}
		
		Shader* GetShader() const
		{
			return m_Shader;
		}
		
		void SetShader(Shader* shader)
		{
			m_Shader = shader;
		}
		
		static void StaticInit();
		static void StaticClean();
		
		static Material* GetErrorMaterial()
		{
			return s_ErrorMaterial;
		}
		
	protected:
		Shader* m_Shader = nullptr;
		
		static Material* s_ErrorMaterial;
		static Material* s_DefaultMaterial;
	};
}

#pragma once

#include <cstdio>
#include "../Object.hpp"
#include "../Asset.hpp"

namespace FishEngine
{
	class Shader;
	
	class Material : public Object
	{
	public:
        InjectClassName(Material, 21);
		
		Material() : Object(Material::ClassID, ClassName)
		{
//			LOGF;
			AssetManager::GetInstance().AddAsset(this);
		}
		Material(const Material&) = delete;
		const Material& operator=(const Material&) = delete;
		
		~Material()
		{
//			LOGF;
			AssetManager::GetInstance().RemoveAsset(this);
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

		static Material* GetDefaultMaterial()
		{
			return s_DefaultMaterial;
		}
		
	protected:
		Shader* m_Shader = nullptr;
		
		static Material* s_ErrorMaterial;
		static Material* s_DefaultMaterial;
	};
}

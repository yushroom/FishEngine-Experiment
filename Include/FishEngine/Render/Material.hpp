#pragma once

#include <cstdio>
#include <vector>
#include <map>

#include "../Object.hpp"
#include "../Asset.hpp"
#include "../Color.hpp"
#include "../Math/Vector2.hpp"

namespace FishEngine
{
	class Shader;
	class Texture;

	struct MaterialTextureProperty
	{
		Texture* m_Texture;
		Vector2 m_Scale;
		Vector2 m_Offset;
	};

	struct MaterialProperties
	{
		std::vector<MaterialTextureProperty> m_TexEnvs;
		std::vector<float> m_Floats;
		std::vector<Color> m_Colors;
	};
	
	class Material : public Object
	{
	public:
		DeclareObject(Material, 21);
		
		Material() : Object(Material::ClassID, ClassName)
		{
			AssetManager::GetInstance().AddAsset(this);
		}
		Material(const Material&) = delete;
		const Material& operator=(const Material&) = delete;
		
		~Material()
		{
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
		std::string m_ShaderKeywords;
		MaterialProperties m_SavedProperties;
		
		static Material* s_ErrorMaterial;
		static Material* s_DefaultMaterial;
	};
}

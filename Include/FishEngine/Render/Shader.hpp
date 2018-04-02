#pragma once

#include <string>
#include <FishEngine/FishEngine.hpp>
#include <FishEngine/Object.hpp>
#include "../Asset.hpp"

namespace FishEngine
{
	enum class ShaderType
	{
		VertexShader,
		FragmentShader,
		GeometryShader,
	};
	
	class Shader : public Object
	{
	public:
		InjectClassName(Shader, 48);
		
		Shader() : Object(Shader::ClassID, ClassName)
		{
			LOGF;
			AssetManager::GetInstance().AddAsset(this);
		}
		Shader(const Shader&) = delete;
		const Shader& operator=(const Shader&) = delete;
		
		~Shader();
		
		static Shader* FromString(const std::string& vs, const std::string& fs);
		
		void Use() const;
		
	private:
		friend class Graphics;
		unsigned int m_GLProgram = 0;
	};
}

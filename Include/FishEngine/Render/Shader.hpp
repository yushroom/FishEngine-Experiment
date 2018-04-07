#pragma once

#include <string>
#include <map>
#include <vector>

#include <FishEngine/FishEngine.hpp>
#include <FishEngine/Object.hpp>
#include "../Asset.hpp"
#include "../Math/Vector2.hpp"

//#include <FishEngine/Render/ShaderProperty.hpp>

namespace FishEngine
{
	enum class ShaderType
	{
		VertexShader,
		FragmentShader,
		GeometryShader,
	};

	struct ShaderUniforms
	{
		std::map<std::string, Matrix4x4> mat4s;
		std::map<std::string, Vector2> vec2s;
		std::map<std::string, Vector3> vec3s;
		std::map<std::string, Vector4> vec4s;
		std::map<std::string, float> floats;
	};

	struct UniformInfo
	{
		uint32_t	type; // type of the variable (float, vec3 or mat4, etc)
		std::string	name;  // variable name in GLSL
		uint32_t	location;
		int			textureBindPoint;
//		bool		binded;
	};

	class ShaderImpl;
	class Texture;
	
	class Shader : public Object
	{
	public:
		InjectClassName(Shader, 48);
		
		Shader();
		Shader(const Shader&) = delete;
		const Shader& operator=(const Shader&) = delete;
		
		~Shader();

		static Shader* FromString(const std::string& shaderStr, bool hasGeometryShader = false);
		static Shader* FromString(const std::string& vs, const std::string& fs);
		static Shader* FromString(const std::string& vs, const std::string& gs, const std::string& fs);

		const std::vector<UniformInfo>& GetUniforms() const;

		void BindUniform(const char* name, float value);
		void BindUniform(const char* name, const Vector2& value);
		void BindUniform(const char* name, const Vector3& value);
		void BindUniform(const char* name, const Vector4& value);
		void BindUniform(const char* name, const Matrix4x4& value);
		void BindTexture(const char* name, Texture* texture);
		
		void Use() const;
		
	private:
		friend class Graphics;
		unsigned int m_GLProgram = 0;
		ShaderImpl* m_impl;
	};
}

#include <FishEngine/Render/Shader.hpp>
#include <FishEngine/Render/GLEnvironment.hpp>
#include <FishEngine/Render/ShaderProperty.hpp>
#include <FishEngine/Render/Pipeline.hpp>
#include <FishEngine/Debug.hpp>
#include <FishEngine/Render/Texture.hpp>

#include <string>
#include <vector>
#include <cassert>

GLuint
CompileShader(GLenum             shader_type,
			  const std::string& shader_str)
{
	const GLchar* shader_c_str = shader_str.c_str();
	GLuint shader = glCreateShader(shader_type);
	assert(shader > 0);
	glShaderSource(shader, 1, &shader_c_str, NULL);
	glCompileShader(shader);
	GLint success = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint infoLogLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> infoLog(infoLogLength + 1);
		glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog.data());
		throw std::runtime_error(infoLog.data());
	}
	return shader;
};


GLuint
LinkShader(GLuint vs,
		   GLuint tcs,
		   GLuint tes,
		   GLuint gs,
		   GLuint fs)
{
	glCheckError();
	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	if (gs != 0)
		glAttachShader(program, gs);
	if (tes != 0) {
		if (tcs != 0) glAttachShader(program, tcs);
		glAttachShader(program, tes);
	}
	glLinkProgram(program);
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLint infoLogLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> infoLog(infoLogLength + 1);
		glGetProgramInfoLog(program, infoLogLength, NULL, infoLog.data());
		throw std::runtime_error(infoLog.data());
	}
	
	glDetachShader(program, vs);
	glDetachShader(program, fs);
	if (gs != 0) {
		glDetachShader(program, gs);
	}
	if (tes != 0) {
		if (tcs != 0) glDetachShader(program, tcs);
		glDetachShader(program, tes);
	}
	
	glCheckError();
	return program;
}


GLuint
LinkShader_tf(GLuint vs,
			  GLuint tcs,
			  GLuint tes,
			  GLuint gs,
			  GLuint fs)
{
	glCheckError();
	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	if (gs != 0)
		glAttachShader(program, gs);
	if (tes != 0) {
		if (tcs != 0) glAttachShader(program, tcs);
		glAttachShader(program, tes);
	}
	const char* const varyings[] = {"OutputPosition", "OutputNormal", "OutputTangent"};
	glCheckError();
	glTransformFeedbackVaryings(program, 3, varyings, GL_SEPARATE_ATTRIBS);
	glCheckError();
	glLinkProgram(program);
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLint infoLogLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> infoLog(infoLogLength + 1);
		glGetProgramInfoLog(program, infoLogLength, NULL, infoLog.data());
		throw std::runtime_error(infoLog.data());
	}

	glDetachShader(program, vs);
	glDetachShader(program, fs);
	if (gs != 0) {
		glDetachShader(program, gs);
	}
	if (tes != 0) {
		if (tcs != 0) glDetachShader(program, tcs);
		glDetachShader(program, tes);
	}

	glCheckError();
	return program;
}

#if 1

const char* GLenumToString(GLenum e)
{
	switch (e) {
		case GL_FLOAT:
			return "GL_FLOAT";
		case GL_FLOAT_VEC2:
			return "GL_FLOAT_VEC2";
		case GL_FLOAT_VEC3:
			return "GL_FLOAT_VEC3";
		case GL_FLOAT_VEC4:
			return "GL_FLOAT_VEC4";
		case GL_FLOAT_MAT4:
			return "GL_FLOAT_MAT4";
		case GL_SAMPLER_2D:
			return "GL_SAMPLER_2D";
		case GL_SAMPLER_2D_ARRAY:
			return "GL_SAMPLER_2D_ARRAY";
		case GL_SAMPLER_3D:
			return "GL_SAMPLER_3D";
		case GL_SAMPLER_CUBE:
			return "GL_SAMPLER_CUBE";
		case GL_SAMPLER_2D_ARRAY_SHADOW:
			return "GL_SAMPLER_2D_ARRAY_SHADOW";
		default:
			abort();
			return "UNKNOWN";
			break;
	}
}

bool UniformIsTexture(GLenum type)
{
	return (type == GL_SAMPLER_2D || type == GL_SAMPLER_2D_ARRAY || type == GL_SAMPLER_3D || type == GL_SAMPLER_CUBE || type == GL_SAMPLER_2D_ARRAY_SHADOW);
}


namespace FishEngine
{
	class ShaderImpl
	{
	public:

		ShaderImpl() = default;

		~ShaderImpl()
		{
//			for (auto& e : m_keywordToGLPrograms)
//			{
//				glDeleteProgram(e.second);
//			}
			if (m_GLProgram != 0)
				glDeleteProgram(m_GLProgram);
		}

		void set(const std::string& shaderText)
		{
			m_shaderTextRaw = shaderText;
		}

//		GLuint CompileAndLink(ShaderKeywords keywords)
		GLuint CompileAndLink()
		{
			//Debug::LogWarning("CompileAndLink %s", m_filePath.c_str());
			auto vs = Compile(ShaderType::VertexShader);
			GLuint gs = 0;
			if (m_hasGeometryShader)
				gs = Compile(ShaderType::GeometryShader);
			auto fs = Compile(ShaderType::FragmentShader);
			//auto glsl_program = LinkShader(vs, 0, 0, gs, fs);
			GLuint glsl_program = LinkShader(vs, 0, 0, gs, fs);
			if (m_transformFeedback)
			{
				glsl_program = LinkShader_tf(vs, 0, 0, gs, fs);
			}
			else
			{
				glsl_program = LinkShader(vs, 0, 0, gs, fs);
			}
//			m_keywordToGLPrograms[keywords] = glsl_program;
			m_GLProgram = glsl_program;
			GetAllUniforms(glsl_program);
			glDeleteShader(vs);
			glDeleteShader(fs);
			if (gs != 0) glDeleteShader(gs);
			glCheckError();
			return glsl_program;
		}

		GLuint glslProgram()
		{
			return m_GLProgram;
		}

		const std::string& shaderTextRaw() const
		{
			return m_shaderTextRaw;
		}

		bool m_transformFeedback = false;

		bool m_hasGeometryShader = false;
		uint32_t m_lineCount = 0;   // for error message

		//private:
		//std::string                         m_filePath;
		std::string                         m_shaderTextRaw;
//		std::map<ShaderKeywords, GLuint>    m_keywordToGLPrograms;
		GLuint m_GLProgram = 0;
		std::map<GLuint, std::vector<UniformInfo>> m_GLProgramToUniforms;
		int m_renderQueue = -1;


		GLuint Compile(ShaderType type)
		{
			std::string text = "#version 410 core\n";
			m_lineCount = 1;

			auto add_macro_definition = [&text, this](const std::string& d)
			{
				text += "#define " + d + "\n";
				this->m_lineCount++;
			};

			GLenum t = GL_VERTEX_SHADER;

			if (type == ShaderType::VertexShader)
			{
				add_macro_definition("VERTEX");
			}
			else if (type == ShaderType::FragmentShader)
			{
				add_macro_definition("FRAGMENT");
				t = GL_FRAGMENT_SHADER;
			}
			else if (type == ShaderType::GeometryShader)
			{
				t = GL_GEOMETRY_SHADER;
				add_macro_definition("GEOMETRY");
			}

//			if (keywords & static_cast<ShaderKeywords>(ShaderKeyword::SkinnedAnimation))
//			{
//				add_macro_definition("_SKINNED");
//			}
//			if (keywords & static_cast<ShaderKeywords>(ShaderKeyword::AmbientIBL))
//			{
//				add_macro_definition("_AMBIENT_IBL");
//			}

			text += m_shaderTextRaw;

			return CompileShader(t, text);
		}

		void GetAllUniforms(GLuint program) noexcept
		{
			std::vector<UniformInfo> uniforms;
			GLuint blockID = glGetUniformBlockIndex(program, "PerCameraUniforms");
			GLint blockSize = 0;
			//assert(blockID != GL_INVALID_INDEX);
			if (blockID != GL_INVALID_INDEX)
			{
				//Debug::Log("%s", "use PerCameraUniforms");
				glUniformBlockBinding(program, blockID, Pipeline::PerCameraUBOBindingPoint);
				glGetActiveUniformBlockiv(program, blockID, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
				assert(blockSize == sizeof(PerCameraUniforms));
			}

			blockID = glGetUniformBlockIndex(program, "PerDrawUniforms");
			blockSize = 0;
			//assert(blockID != GL_INVALID_INDEX);
			if (blockID != GL_INVALID_INDEX)
			{
				//Debug::Log("%s", "use PerDrawUniforms");
				glUniformBlockBinding(program, blockID, Pipeline::PerDrawUBOBindingPoint);
				glGetActiveUniformBlockiv(program, blockID, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
				assert(blockSize == sizeof(PerDrawUniforms));
			}

			blockID = glGetUniformBlockIndex(program, "LightingUniforms");
			//assert(blockID != GL_INVALID_INDEX);
			if (blockID != GL_INVALID_INDEX)
			{
				//Debug::Log("%s", "use LightingUniforms");
				glUniformBlockBinding(program, blockID, Pipeline::LightingUBOBindingPoint);
				glGetActiveUniformBlockiv(program, blockID, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
				assert(blockSize == sizeof(LightingUniforms));
			}

			blockID = glGetUniformBlockIndex(program, "Bones");
			//assert(blockID != GL_INVALID_INDEX);
			if (blockID != GL_INVALID_INDEX)
			{
				//Debug::Log("%s", "use Bones");
				glUniformBlockBinding(program, blockID, Pipeline::BonesUBOBindingPoint);
				glGetActiveUniformBlockiv(program, blockID, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
				assert(blockSize == sizeof(Bones));
			}

			GLint count;
			GLint size; // size of the variable
			GLenum type; // type of the variable (float, vec3 or mat4, etc)
			const GLsizei bufSize = 32; // maximum name length
			GLchar name[bufSize]; // variable name in GLSL
			GLsizei length; // name length

			glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);

			int texture_count = 0;
			for (int i = 0; i < count; i++)
			{
				glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, name);
				GLint loc = glGetUniformLocation(program, name);
				if (loc != GL_INVALID_INDEX)	// this uniform is in a uniform block, does not have locations
				{
					//Debug::Log("Uniform #%d Type: %s Name: %s Loc: %d", i, GLenumToString(type), name, loc);
					UniformInfo u;
					u.type = type;
					u.name = name;
					u.location = loc;
					if (UniformIsTexture(type))
					{
						u.textureBindPoint = texture_count;
						texture_count++;
					}
					else {
						u.textureBindPoint = -1;
					}
//					u.binded = false;
					uniforms.emplace_back(u);
				}
			}
			m_GLProgramToUniforms[program] = uniforms;
		}
	};

} /* FishEngine */

#endif


namespace FishEngine
{
	unsigned int Compile(ShaderType type)
	{
		return 0;
	}

	Shader::Shader() : Object(Shader::ClassID, ClassName)
	{
		AssetManager::GetInstance().AddAsset(this);
		m_impl = new ShaderImpl;
	}
	
	Shader::~Shader()
	{
		if (m_GLProgram != 0)
		{
			glDeleteProgram(m_GLProgram);
		}

		delete m_impl;
		AssetManager::GetInstance().RemoveAsset(this);
	}

	Shader* Shader::FromString(const std::string& shaderStr, bool hasGeometryShader)
	{
		auto s = new Shader;
		s->m_impl->set(shaderStr);
		s->m_impl->m_hasGeometryShader = hasGeometryShader;
		s->m_impl->CompileAndLink();
		s->m_GLProgram = s->m_impl->glslProgram();
		return s;
	}
	
	Shader* Shader::FromString(const std::string& vs, const std::string& fs)
	{
		auto s = new Shader;
		auto v = CompileShader(GL_VERTEX_SHADER, vs);
		auto f = CompileShader(GL_FRAGMENT_SHADER, fs);
		auto program = LinkShader(v, 0, 0, 0, f);
		s->m_GLProgram = program;
		return s;
	}

	Shader* Shader::FromString(const std::string& vs, const std::string& gs, const std::string& fs)
	{
		auto s = new Shader;
		auto v = CompileShader(GL_VERTEX_SHADER, vs);
		auto g = CompileShader(GL_GEOMETRY_SHADER, gs);
		auto f = CompileShader(GL_FRAGMENT_SHADER, fs);
		auto program = LinkShader(v, 0, 0, g, f);
		s->m_GLProgram = program;
		return s;
	}
	
	void Shader::Use() const
	{
		assert(m_GLProgram != 0);
		glUseProgram(m_GLProgram);
	}

	const std::vector<UniformInfo>& Shader::GetUniforms() const
	{
		return m_impl->m_GLProgramToUniforms[m_GLProgram];
	}

	void Shader::BindUniform(const char* name, float value)
	{
		auto uniforms = this->GetUniforms();
		for (auto& u : uniforms)
		{
			if (u.name == name)
			{
				glProgramUniform1f(this->m_GLProgram, u.location, value);
				return;
			}
		}
		LogWarning(Format( "Uniform {} not found!", name ));
	}

	void Shader::BindUniform(const char* name, const Vector2& value)
	{
		auto uniforms = this->GetUniforms();
		for (auto& u : uniforms)
		{
			if (u.name == name)
			{
				glProgramUniform2fv(m_GLProgram, u.location, 1, value.data());
				return;
			}
		}
		LogWarning(Format( "Uniform {} not found!", name ));
	}

	void Shader::BindUniform(const char* name, const Vector3& value)
	{
		auto uniforms = this->GetUniforms();
		for (auto& u : uniforms)
		{
			if (u.name == name)
			{
				glProgramUniform3fv(m_GLProgram, u.location, 1, value.data());
				return;
			}
		}
		LogWarning(Format( "Uniform {} not found!", name ));
	}

	void Shader::BindUniform(const char* name, const Vector4& value)
	{
		auto uniforms = this->GetUniforms();
		for (auto& u : uniforms)
		{
			if (u.name == name)
			{
				assert(u.type == GL_FLOAT_VEC4);
				glProgramUniform4fv(m_GLProgram, u.location, 1, value.data());
				return;
			}
		}
		LogWarning(Format( "Uniform {} not found!", name ));
	}

	void Shader::BindUniform(const char* name, const Matrix4x4& value)
	{
		auto uniforms = this->GetUniforms();
		for (auto& u : uniforms)
		{
			if (u.name == name)
			{
//				glProgramUniform4fv(m_GLProgram, u.location, 1, value.data());
				glProgramUniformMatrix4fv(m_GLProgram, u.location, 1, GL_TRUE, value.data());
				return;
			}
		}
		LogWarning(Format( "Uniform {} not found!", name ));
	}

	void Shader::BindTexture(const char* name, Texture* texture)
	{
		auto uniforms = this->GetUniforms();
		for (auto& u : uniforms)
		{
			if (name == u.name)
			{
				GLenum type = GL_TEXTURE_2D;
				if (u.type == GL_SAMPLER_CUBE)
					type = GL_TEXTURE_CUBE_MAP;
				else if (u.type == GL_SAMPLER_2D_ARRAY || u.type == GL_SAMPLER_2D_ARRAY_SHADOW)
					type = GL_TEXTURE_2D_ARRAY;
				//BindUniformTexture(u.name.c_str(), it->second->GLTexuture(), texture_id, type);
				glActiveTexture(GLenum(GL_TEXTURE0 + u.textureBindPoint));
				glBindTexture(type, texture->GetNativeTexturePtr());
				glCheckError();
				return;
			}
		}
		LogWarning(Format( "Uniform {} not found!", name ));
	}
}

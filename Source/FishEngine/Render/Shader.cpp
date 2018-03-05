#include <FishEngine/Render/Shader.hpp>
#include <FishEngine/Render/GLEnvironment.hpp>

#include <string>
#include <vector>
#include <cassert>

GLuint
CompileShader(
			  GLenum             shader_type,
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


namespace FishEngine
{
	unsigned int Compile(ShaderType type)
	{
		return 0;
	}
	
	Shader::~Shader()
	{
		if (m_GLProgram != 0)
		{
			glDeleteProgram(m_GLProgram);
		}
		
		Object::s_objects[ClassID].erase(this);
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
	
	void Shader::Use() const
	{
		assert(m_GLProgram != 0);
		glUseProgram(m_GLProgram);
	}
}

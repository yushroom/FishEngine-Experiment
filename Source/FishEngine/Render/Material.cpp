#include <FishEngine/Render/Material.hpp>
#include <FishEngine/Render/Shader.hpp>

const char error_vs[] = R"(#version 410 core
#define PositionIndex 0
layout (location = PositionIndex)   in vec3 InputPositon;
uniform mat4 MATRIX_MVP;

void main()
{
	gl_Position = MATRIX_MVP * vec4(InputPositon, 1);
}
)";

const char error_fs[] = R"(#version 410 core
out vec4 fragColor;
void main()
{
	fragColor = vec4(1, 0, 1, 1);
}
)";

namespace FishEngine
{
	Material* Material::s_errorMaterial = nullptr;
	
	void Material::StaticInit()
	{
		s_errorMaterial = new Material();
		s_errorMaterial->m_shader = Shader::FromString(error_vs, error_fs);
	}
	
	void Material::StaticClean()
	{
		delete s_errorMaterial->m_shader;
		delete s_errorMaterial;
	}
}

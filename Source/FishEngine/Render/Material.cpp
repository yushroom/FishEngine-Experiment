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
	Material* Material::s_ErrorMaterial = nullptr;
	
	void Material::StaticInit()
	{
		s_ErrorMaterial = new Material();
		s_ErrorMaterial->m_Shader = Shader::FromString(error_vs, error_fs);
	}
	
	void Material::StaticClean()
	{
		delete s_ErrorMaterial->m_Shader;
		delete s_ErrorMaterial;
	}
}

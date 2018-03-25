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

const char default_vs[] = R"(#version 410 core
#define PositionIndex 0
#define NormalIndex 1
#define TangentIndex 2
#define UVIndex 3
#define BoneIndexIndex 4
#define BoneWeightIndex 5
layout(location = PositionIndex)   in vec3 InputPositon;
layout(location = NormalIndex)     in vec3 InputNormal;
layout(location = TangentIndex)    in vec3 InputTangent;
layout(location = UVIndex)         in vec2 InputUV;
uniform mat4 MATRIX_MVP;
uniform mat4 MATRIX_M;

out vec3 normal;

void main()
{
	gl_Position = MATRIX_MVP * vec4(InputPositon, 1);
	//normal = InputNormal*0.5+0.5;
	normal = mat3(MATRIX_M) * InputNormal;
	//normal = normalize(normal);
	//normal = normalize(InputNormal);
	//color = vec3(gl_Position.z);
})";

const char default_fs[] = R"(#version 410 core
in vec3 normal;
//const vec3 L = normalize(vec3(1, 1, 0));
uniform vec3 LightDir;
out vec4 fragColor;
void main()
{
	vec3 N = normalize(normal);
	//fragColor = vec4(normal, 1);
	float ndotl = clamp(dot(N, -LightDir), 0, 1);
	fragColor = vec4(vec3(ndotl), 1);
	//fragColor = vec4(N*0.5+0.5, 1);
	//fragColor = vec4(vec3(gl_FragCoord.z*5), 1);
})";

namespace FishEngine
{
	Material* Material::s_ErrorMaterial = nullptr;
	Material* Material::s_DefaultMaterial = nullptr;
	
	void Material::StaticInit()
	{
		s_ErrorMaterial = new Material();
		s_ErrorMaterial->m_Shader = Shader::FromString(error_vs, error_fs);

		s_DefaultMaterial = new Material();
		s_DefaultMaterial->m_Shader = Shader::FromString(default_vs, default_fs);
	}
	
	void Material::StaticClean()
	{
		delete s_ErrorMaterial->m_Shader;
		delete s_ErrorMaterial;

		delete s_DefaultMaterial->m_Shader;
		delete s_DefaultMaterial;
	}
}

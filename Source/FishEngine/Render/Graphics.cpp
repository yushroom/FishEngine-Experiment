#include <FishEngine/Render/Graphics.hpp>
#include <FishEngine/Render/Mesh.hpp>
#include <FishEngine/Render/Material.hpp>
#include <FishEngine/Render/Shader.hpp>
#include <FishEngine/Math/Matrix4x4.hpp>
#include <FishEngine/Render/GLEnvironment.hpp>
#include <FishEngine/Component/Camera.hpp>
#include <FishEngine/Component/Light.hpp>
#include <FishEngine/Transform.hpp>
#include <cassert>
#include <cstdio>

namespace FishEngine
{
	void Graphics::DrawMesh(Mesh* mesh, Material* material, int subMeshIndex, Camera* camera, const Matrix4x4& mat, Light* light)
	{
		auto shader = material->GetShader();
		shader->Use();
//		auto vp = camera->projectionMatrix() * v;
		auto mv = camera->GetWorldToCameraMatrix() * mat;
		auto mvp = camera->GetProjectionMatrix() * mv;
		auto lightDir = light->GetTransform()->GetForward();
		auto loc1 = glGetUniformLocation(shader->m_GLProgram, "MATRIX_MVP");
		auto loc2 = glGetUniformLocation(shader->m_GLProgram, "MATRIX_M");
		auto loc3 = glGetUniformLocation(shader->m_GLProgram, "LightDir");
//		assert(loc1 != GL_INVALID_INDEX);
//		assert(loc2 != GL_INVALID_INDEX);
//		assert(loc3 != GL_INVALID_INDEX);
#if USE_GLM
		auto need_transpose = GL_FALSE;
#else
		auto need_transpose = GL_TRUE;
#endif
		if (loc1 != GL_INVALID_INDEX)
			glProgramUniformMatrix4fv(shader->m_GLProgram, loc1, 1, need_transpose, mvp.data());
		if (loc2 != GL_INVALID_INDEX)
			glProgramUniformMatrix4fv(shader->m_GLProgram, loc2, 1, need_transpose, mat.data());
		if (loc3 != GL_INVALID_INDEX)
			glProgramUniform3fv(shader->m_GLProgram, loc3, 1, lightDir.data());
		mesh->Render(subMeshIndex);
	}
}

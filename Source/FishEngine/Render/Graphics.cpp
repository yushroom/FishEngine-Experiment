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

#include <FishEngine/Render/GLEnvironment.hpp>

namespace FishEngine
{
	void Graphics::DrawMesh(Mesh* mesh, Material* material, int subMeshIndex, Camera* camera, const Matrix4x4& mat, Light* light)
	{
		auto shader = material->GetShader();
		shader->Use();

		const auto& ObjectToWorld = mat;
		auto mv = camera->GetWorldToCameraMatrix() * mat;
		auto mvp = camera->GetProjectionMatrix() * mv;
		auto vp = camera->GetProjectionMatrix() * camera->GetWorldToCameraMatrix();
		auto lightDir = light->GetTransform()->GetForward().normalized();

#if USE_GLM
		auto need_transpose = GL_FALSE;
#else
		auto need_transpose = GL_TRUE;
#endif


		if (material->GetName() == "Default-Material")
		{
			auto loc1 = glGetUniformLocation(shader->m_GLProgram, "MATRIX_MVP");
			auto loc2 = glGetUniformLocation(shader->m_GLProgram, "MATRIX_M");
			auto loc3 = glGetUniformLocation(shader->m_GLProgram, "LightDir");
			assert(loc1 != GL_INVALID_INDEX);
			assert(loc2 != GL_INVALID_INDEX);
			assert(loc3 != GL_INVALID_INDEX);
			if (loc1 != GL_INVALID_INDEX)
				glProgramUniformMatrix4fv(shader->m_GLProgram, loc1, 1, need_transpose, mvp.data());
			if (loc2 != GL_INVALID_INDEX)
				glProgramUniformMatrix4fv(shader->m_GLProgram, loc2, 1, need_transpose, mat.data());
			if (loc3 != GL_INVALID_INDEX)
				glProgramUniform3fv(shader->m_GLProgram, loc3, 1, lightDir.data());
		}
		else if (material->GetName() == "Default-Skybox")
		{
			auto loc1 = glGetUniformLocation(shader->m_GLProgram, "_WorldSpaceLightPos0");
			auto loc2 = glGetUniformLocation(shader->m_GLProgram, "hlslcc_mtx4x4unity_ObjectToWorld");
			auto loc3 = glGetUniformLocation(shader->m_GLProgram, "hlslcc_mtx4x4unity_MatrixVP");
			auto loc4 = glGetUniformLocation(shader->m_GLProgram, "_Exposure");
			auto loc5 = glGetUniformLocation(shader->m_GLProgram, "_GroundColor");
			auto loc6 = glGetUniformLocation(shader->m_GLProgram, "_SkyTint");
			auto loc7 = glGetUniformLocation(shader->m_GLProgram, "_AtmosphereThickness");
			assert(loc1 != GL_INVALID_INDEX);
			assert(loc2 != GL_INVALID_INDEX);
			assert(loc3 != GL_INVALID_INDEX);
			assert(loc4 != GL_INVALID_INDEX);
			assert(loc5 != GL_INVALID_INDEX);
			assert(loc6 != GL_INVALID_INDEX);
			assert(loc7 != GL_INVALID_INDEX);


			auto L = -light->GetTransform()->GetForward().normalized();
			Vector4 _WorldSpaceLightPos0(L.x, L.y, L.z, 0.0f);
			glProgramUniform4fv(shader->m_GLProgram, loc1, 1, _WorldSpaceLightPos0.data());

//			Matrix4x4 hlslcc_mtx4x4unity_ObjectToWorld = ObjectToWorld;
			Matrix4x4 hlslcc_mtx4x4unity_ObjectToWorld = ObjectToWorld;
			if (need_transpose)
				hlslcc_mtx4x4unity_ObjectToWorld = ObjectToWorld.transpose();
			glProgramUniform4fv(shader->m_GLProgram, loc2, 4, hlslcc_mtx4x4unity_ObjectToWorld.data());

			constexpr float epsilon = 1e-6;
			float near = camera->GetNearClipPlane();
			float far = camera->GetFarClipPlane();
			Matrix4x4 p;
			if (!camera->GetOrthographic())
				p = camera->GetProjectionMatrix();
			p[2][2] = epsilon - 1;
			p[2][3] = (epsilon - 2) * near;
			p[3][2] = -1.0f;
			Matrix4x4 hlslcc_mtx4x4unity_MatrixVP;
			if (camera->GetOrthographic())
				hlslcc_mtx4x4unity_MatrixVP = p;
			else
				hlslcc_mtx4x4unity_MatrixVP = p * camera->GetWorldToCameraMatrix();
//			puts("V:");
//			puts(camera->GetWorldToCameraMatrix().ToString().c_str());
////			puts("Projection");
////			puts(camera->GetProjectionMatrix().ToString().c_str());
//			puts("P");
//			puts(p.ToString().c_str());
//			puts("VP");
//			puts(hlslcc_mtx4x4unity_MatrixVP.ToString().c_str());
////			puts("VP2");
////			puts(vp.ToString().c_str());
////			exit(0);
			if (need_transpose)
				hlslcc_mtx4x4unity_MatrixVP = vp.transpose();
			glProgramUniform4fv(shader->m_GLProgram, loc3, 4, hlslcc_mtx4x4unity_MatrixVP.data());

			float _Exposure = 1.3f;
			glProgramUniform1f(shader->m_GLProgram, loc4, _Exposure);

			Vector3 _GroundColor (0.369, 0.349, .341);
			glProgramUniform3fv(shader->m_GLProgram, loc5, 1, _GroundColor.data());

			Vector3 _SkyTint (0.5f, 0.5f, 0.5f);
			glProgramUniform3fv(shader->m_GLProgram, loc6, 1, _SkyTint.data());

			float _AtmosphereThickness = 1.0f;
			glProgramUniform1f(shader->m_GLProgram, loc7, _AtmosphereThickness);

			glCheckError();
		}
		else if (material->GetName() =="Default-Error")
		{
			auto loc1 = glGetUniformLocation(shader->m_GLProgram, "MATRIX_MVP");
			auto loc2 = glGetUniformLocation(shader->m_GLProgram, "MATRIX_M");
			auto loc3 = glGetUniformLocation(shader->m_GLProgram, "LightDir");
			if (loc1 != GL_INVALID_INDEX)
				glProgramUniformMatrix4fv(shader->m_GLProgram, loc1, 1, need_transpose, mvp.data());
			if (loc2 != GL_INVALID_INDEX)
				glProgramUniformMatrix4fv(shader->m_GLProgram, loc2, 1, need_transpose, mat.data());
			if (loc3 != GL_INVALID_INDEX)
				glProgramUniform3fv(shader->m_GLProgram, loc3, 1, lightDir.data());
		}
		else {
			abort();
		}

		mesh->Render(subMeshIndex);
	}
}

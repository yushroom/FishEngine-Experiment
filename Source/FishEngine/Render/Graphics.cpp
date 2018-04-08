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

#ifdef _WIN32
#undef near
#undef far
#endif

namespace FishEngine
{
	void  Graphics::DrawMesh(Mesh* mesh, Material* material, int subMeshIndex)
	{
		auto shader = material->GetShader();
		shader->Use();
		mesh->Render(subMeshIndex);
	}

	void Graphics::DrawMesh(Mesh* mesh, Material* material, int subMeshIndex, Camera* camera, const Matrix4x4& mat, Light* light)
	{
//		Pipeline::BindCamera(camera);
//		Pipeline::BindLight(light);
//		Pipeline::UpdatePerDrawUniforms(model);
//		DrawMesh(mesh, material, sub)
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


		if (material->GetName() == "Default-Skybox")
		{
			auto uniforms = shader->GetUniforms();

			float _Exposure = 1.3f;
			Vector3 _GroundColor (0.369, 0.349, .341);
			Vector3 _SkyTint (0.5f, 0.5f, 0.5f);
			float _AtmosphereThickness = 1.0f;
			shader->BindUniform("_Exposure", _Exposure);
			shader->BindUniform("_GroundColor", _GroundColor);
			shader->BindUniform("_SkyTint", _SkyTint);
			shader->BindUniform("_AtmosphereThickness", _AtmosphereThickness);


			auto loc1 = glGetUniformLocation(shader->m_GLProgram, "_WorldSpaceLightPos0");
			auto loc2 = glGetUniformLocation(shader->m_GLProgram, "hlslcc_mtx4x4unity_ObjectToWorld");
			auto loc3 = glGetUniformLocation(shader->m_GLProgram, "hlslcc_mtx4x4unity_MatrixVP");
			assert(loc1 != GL_INVALID_INDEX);
			assert(loc2 != GL_INVALID_INDEX);
			assert(loc3 != GL_INVALID_INDEX);


			auto L = -light->GetTransform()->GetForward().normalized();
			Vector4 _WorldSpaceLightPos0(L.x, L.y, L.z, 0.0f);
			glProgramUniform4fv(shader->m_GLProgram, loc1, 1, _WorldSpaceLightPos0.data());

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
			if (need_transpose)
				hlslcc_mtx4x4unity_MatrixVP = vp.transpose();
			glProgramUniform4fv(shader->m_GLProgram, loc3, 4, hlslcc_mtx4x4unity_MatrixVP.data());

			glCheckError();
		}
		else if (material->GetName() =="Default-Error")
		{
			shader->BindUniform("MATRIX_MVP", mvp);
		}
		else {
			abort();
		}

		mesh->Render(subMeshIndex);
	}
}

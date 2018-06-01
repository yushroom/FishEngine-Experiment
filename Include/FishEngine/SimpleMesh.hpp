#pragma once
#include "Render/GLEnvironment.hpp"
#include <vector>

namespace FishEngine
{
	class Gizmos;

	class SimpleMesh
	{
	public:
		SimpleMesh(const float* positionBuffer, uint32_t vertexCount, GLenum drawMode);

		void Render() const;

	private:
		friend class Gizmos;
		std::vector<float> m_positionBuffer;

		GLenum m_drawMode = GL_LINES;	// mode in glDrawArrays
		GLuint m_VAO = 0;
		GLuint m_VBO = 0;
	};

	class DynamicMesh
	{
	public:
		//InjectClassName(DynamicMesh)

		DynamicMesh() = default;

		void Render(const float* positionBuffer, uint32_t vertexCount, GLenum drawMode);

	private:
		GLenum m_drawMode = GL_LINES;   // mode in glDrawArrays
		GLuint m_VAO = 0;
		GLuint m_VBO = 0;
	};
}

#include <FishEngine/SimpleMesh.hpp>
#include <FishEngine/Math/Matrix4x4.hpp>
#include <FishEngine/Math/Vector3.hpp>
#include <FishEngine/Render/ShaderVariables.hpp>

namespace FishEngine
{
	SimpleMesh::SimpleMesh(const float* positionBuffer, uint32_t vertexCount, GLenum drawMode)
		: m_positionBuffer(positionBuffer, positionBuffer + vertexCount * 3),
		m_drawMode(drawMode)
	{
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_positionBuffer.size() * sizeof(GLfloat), m_positionBuffer.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(PositionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(PositionIndex);
	}

	//void SimpleMesh::BindNewBuffer(const float* positionBuffer, uint32_t vertexCount)
	//{
	//    glBindVertexArray(m_VAO);
	//    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//    glBufferData(GL_ARRAY_BUFFER, vertexCount*3*sizeof(GLfloat), positionBuffer, GL_DYNAMIC_DRAW);
	//    //glVertexAttribPointer(PositionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//    //glEnableVertexAttribArray(PositionIndex);
	//}

	void SimpleMesh::Render() const
	{
		glBindVertexArray(m_VAO);
		glDrawArrays(m_drawMode, 0, static_cast<GLsizei>(m_positionBuffer.size() / 3));
		glBindVertexArray(0);
	}

	void DynamicMesh::Render(const float* positionBuffer, uint32_t vertexCount, GLenum drawMode)
	{
		if (m_VAO == 0)
			glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		if (m_VBO == 0)
			glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), positionBuffer, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(PositionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(PositionIndex);
		glDrawArrays(m_drawMode, 0, vertexCount);
		glBindVertexArray(0);
	}
}

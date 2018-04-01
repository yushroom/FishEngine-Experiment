#pragma once

#include <vector>
#include "../Object.hpp"
#include "../Math/Vector2.hpp"
#include "../Math/Vector3.hpp"
#include "../Math/Matrix4x4.hpp"
#include "../Math/Bounds.hpp"
#include "BoneWeight.hpp"

namespace FishEngine
{
	class Mesh : public Object
	{
		friend class RawMesh;
	public:
		InjectClassName(Mesh, 43);

		Mesh() : Object(Mesh::ClassID, ClassName)
		{
			LOGF;
		}
		
		~Mesh()
		{
			LOGF;
		}
		
		Mesh(std::vector<Vector3>	&& vertices,
			 std::vector<Vector3>	&& normals,
			 std::vector<Vector2>	&& uv,
			 std::vector<Vector3>	&& tangents,
			 std::vector<uint32_t>	&& triangles);
		
		void Clear();
		
		// -1: render all sub meshes
		void Render(int subMeshIndex = -1);
		
//		void RenderSkinned();
		void UploadMeshData(bool markNoLogerReadable = true);
		
		// Recalculate the bounding volume of the Mesh from the vertices.
		void RecalculateBounds();
		
		static Mesh* FromTextFile(const std::string &str);
		
	private:
		int						m_subMeshCount = 1;
		std::vector<Vector3>    m_vertices;
		std::vector<Vector3>    m_normals;
		std::vector<Vector2>    m_uv;
		std::vector<Vector3>    m_tangents;
		std::vector<uint32_t>   m_triangles;
		std::vector<uint32_t>	m_subMeshIndexOffset;	// index start
		
		// The bind poses. The bind pose at each index refers to the bone with the same index.
		// The bind pose is the inverse of the transformation matrix of the bone, when the bone is in the bind pose.
		std::vector<Matrix4x4>  m_bindposes;
		
		std::vector<std::string> m_boneNames;	// same size with m_binposes;
		
		// The bone weights of each vertex.
		// The size of the array is either the same as vertexCount or empty.
		// Each vertex can be affected by up to 4 different bones.All 4 bone weights should sum up to 1.
		std::vector<BoneWeight> m_boneWeights;
		
	public:
		bool m_skinned = false; // temp
		bool m_isReadable = false;
		bool m_uploaded = false;
		uint32_t m_vertexCount = 0;
		uint32_t m_triangleCount = 0;
		Bounds m_bounds;
		unsigned int m_VAO = 0;
//		unsigned int m_VBO = 0;
		unsigned int m_indexVBO = 0;
		unsigned int m_positionVBO = 0;
		unsigned int m_normalVBO = 0;
		unsigned int m_uvVBO = 0;
		unsigned int m_tangentVBO = 0;
		unsigned int m_boneIndexVBO = 0;
		unsigned int m_boneWeightVBO = 0;
		unsigned int m_TFBO = 0;				// transform feedback buffer object, for Animation
		unsigned int m_animationInputVAO = 0;	// for transform feedback
		unsigned int m_animationOutputPositionVBO = 0;
		unsigned int m_animationOutputNormalVBO = 0;
		unsigned int m_animationOutputTangentVBO = 0;
		
	private:
		void GenerateBuffer();
		void BindBuffer();
	};
}

#pragma once

#include "AssetImporter.hpp"
#include <FishEngine/Render/Mesh.hpp>
//#include <FishEngine/Scene.hpp>
#include <FishEngine/Render/Material.hpp>
//#include <FishEngine/Component/MeshFilter.hpp>
//#include <FishEngine/Animator.hpp>
//#include <FishEngine/PrimitiveType.hpp>

//class ModelImporterInspector;

#include <map>

namespace FishEditor
{
	// Vertex normal generation options for ModelImporter.
	enum class ModelImporterNormals
	{
		Import,     // Import vertex normals from model file(default).
		Calculate,  // Calculate vertex normals.
		None,       // Do not import vertex normals.
	};


	// Vertex tangent generation options for ModelImporter.
	enum class ModelImporterTangents
	{
		Import,         // Import vertex tangents from model file.
		//CalculateMikk,// Calculate tangents using MikkTSpace(default).
		Calculate,      // Calculate tangents.
		None,           // Do not import vertex tangents.
	};
	
	// Material naming options for ModelImporter.
	enum class ModelImporterMaterialName
	{
		BasedOnTextureName,                 // Use material names in the form <textureName>.mat.
		BasedOnMaterialName,                // Use a material name of the form <materialName>.mat.
		BasedOnModelNameAndMaterialName,    // Use material names in the form <modelFileName>-<materialName>.mat.
	};
	
	enum class ModelImporterAnimationType
	{
		None,       // Generate no animation data.
		//Legacy,     // Generate a legacy animation type.
		Generic,    // Generate a generic animator.
		Human,      // Generate a human animator.
	};
	
	enum class ModelImporterMaterialSearch
	{
		Local,          // Search in local Materials folder.
		RecursiveUp,    // Recursive-up search in Materials folders.
		Everywhere,     // Search in all project.
	};
	
	enum class ModelImporterMeshCompression
	{
		Off,        // No mesh compression (default).
		Low,        // Low amount of mesh compression.
		Medium,     // Medium amount of mesh compression.
		High,       // High amount of mesh compression.
	};
	
	enum class ModelImporterAnimationCompression
	{
		Off,                                // No animation compression.
		KeyframeReduction,                  // Perform keyframe reduction.
		KeyframeReductionAndCompression,    // Perform keyframe reduction and compression.
		Optimal,                            //Perform keyframe reduction and choose the best animation curve representation at runtime to reduce memory footprint (default).
	};

	struct ModelImporterMesh
	{
		float globalScale = 1;
		bool useFileScale = true;
	};
	
	class ModelImporter : public AssetImporter
	{
	public:
		enum { ClassID = 1040 };
		ModelImporter(int classID = ClassID) : AssetImporter(classID) { }
		
//		ModelImporter& operator=(ModelImporter const & rhs);
		
		float GetGlobalScale() const { return m_GlobalScale; }
		void SetGlobalScale(float value) { m_GlobalScale = value; }

		bool GetUseFileScale() const { return m_UseFileScale; }
		void SetUseFileScale(bool value) { m_UseFileScale = value; }

		float GetFileScale() const { return m_FileScale; }
		void SetFileScale(float value) { m_FileScale = value; }
		
		//ModelPtr LoadFromFile( const FishEngine::Path& path );

		void setImportNormals( ModelImporterNormals importNormals )
		{
			m_importNormals = importNormals;
		}

		void setImportTangents( ModelImporterTangents importTangents )
		{
			m_importTangents = importTangents;
		}

	protected:

		float GetScale()
		{
			if (m_UseFileScale)
				return m_GlobalScale * m_FileScale;
			return m_GlobalScale;
		}

		
//		friend class Inspector;
//		friend class MainEditor;
//		friend class ::ModelImporterInspector;

//		static void Init();

		// Global scale factor for importing.
		float m_GlobalScale = 1.0f;
		

		bool m_UseFileScale = true;

		// File scale factor (if available) or default one. (Read-only).
		// fileSscale is determined by model file and can not be modified by user.
//		Meta(NonSerializable)
		float m_FileScale = 1.0f;

		// Vertex normal import options.
		ModelImporterNormals m_importNormals    = ModelImporterNormals::Import;

		// Vertex tangent import options.
		ModelImporterTangents m_importTangents  = ModelImporterTangents::Calculate;
		
		// Existing material search setting.
		ModelImporterMaterialSearch m_materialSearch;

		// remove dummy nodes
//		Meta(NonSerializable)
		std::map<std::string, std::map<std::string, FishEngine::Matrix4x4>> m_nodeTransformations;

		//ModelImporterMesh m_Meshes;

	}; // end of class ModelImporter
}

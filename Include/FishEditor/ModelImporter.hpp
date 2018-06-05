#pragma once

#include "AssetImporter.hpp"
#include <FishEngine/Render/Mesh.hpp>
//#include <FishEngine/Scene.hpp>
#include <FishEngine/Render/Material.hpp>
#include <FishEngine/Animation/HumanDescription.hpp>
//#include <FishEngine/Component/MeshFilter.hpp>
//#include <FishEngine/Animator.hpp>
//#include <FishEngine/PrimitiveType.hpp>

//class ModelImporterInspector;

#include <map>

namespace FishEditor
{
	class AssetDatabase;

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
		Legacy,     // Generate a legacy animation type.
		Generic,    // Generate a generic animator.
		Human,      // Generate a human animator.
	};
	
	enum class ModelImporterMaterialSearch
	{
		Local,          // Search in local Materials folder.
		RecursiveUp,    // Recursive-up search in Materials folders.
		Everywhere,     // Search in all project.
	};
	
	struct ModelImporterMaterial
	{
		bool importMaterials = true;
		std::string materialName;
		ModelImporterMaterialSearch materialSearch;
	};
	
	struct ModelImporterClipAnimation
	{
		std::string name;
		std::string takeName;
		float firstFrame = 0;
		float lastFrame = 0;
	};
	
	struct ModelImporterAnimation
	{
		std::vector<ModelImporterClipAnimation> clipAnimations;
		bool isReadable = true;
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
		float globalScale = 1.0f;
		bool addColliders = false;
		bool importBlendShapes = true;
		bool swapUVChannels = false;
		bool useFileUnits = true;
		bool useFileScale = true;
		float fileScale = 1.0f;
	};
	
	class ModelImporter : public AssetImporter
	{
	public:
		DeclareObject(ModelImporter, 1040);

		ModelImporter() : AssetImporter(ClassID, ClassName) { }
		ModelImporter(int classID, const char* className) : AssetImporter(classID, className) { }
		
//		ModelImporter& operator=(ModelImporter const & rhs);
		
		// Global scale factor for importing.
		float GetGlobalScale() const { return meshes.globalScale; }
		void SetGlobalScale(float value) { meshes.globalScale = value; }

		bool GetUseFileScale() const { return meshes.useFileScale; }
		void SetUseFileScale(bool value) { meshes.useFileScale = value; }

		// File scale factor (if available) or default one. (Read-only).
		// fileSscale is determined by model file and can not be modified by user.
		float GetFileScale() const { return meshes.fileScale; }

		void setImportNormals( ModelImporterNormals importNormals )
		{
			m_importNormals = importNormals;
		}

		void setImportTangents( ModelImporterTangents importTangents )
		{
			m_importTangents = importTangents;
		}

		const std::map<int64_t, FishEngine::Object*>& GetFileIDToObject() const
		{
			return m_FileIDToObject;
		}

		virtual void Import() override { }

	protected:
		friend class AssetDatabase;
		friend class AssetImporter;
		
		std::map<uint32_t, std::string> m_FileIDToRecycleName;

//		std::map<int64_t, FishEngine::Object*> m_FileIDToObject;

		float GetScale() const
		{
			if (GetUseFileScale())
				return GetGlobalScale() * GetFileScale();
			return GetFileScale();
		}

		
//		friend class Inspector;
//		friend class MainEditor;
//		friend class ::ModelImporterInspector;

//		static void Init();

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
		
//		std::map<int64_t, std::string> fileIDToRecycleName;
		ModelImporterAnimation animations;
		ModelImporterMesh meshes;
		bool importAnimation = true;
		FishEngine::HumanDescription humanDescription;
		ModelImporterAnimationType animationType = ModelImporterAnimationType::Generic;

	}; // end of class ModelImporter
}

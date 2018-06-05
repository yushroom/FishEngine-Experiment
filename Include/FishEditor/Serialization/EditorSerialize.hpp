#pragma once

#include <FishEngine/Serialization/Archive.hpp>
#include <FishEngine/Serialization/Serialize.hpp>

//namespace FishEngine
//{
//	class InputArchive;
//	class OutputArchive;
//}

namespace FishEditor
{
	struct ModelImporterClipAnimation;
	struct ModelImporterAnimation;
	struct ModelImporterMesh;
}

namespace FishEditor
{
	// ModelImporterClipAnimation
	FishEngine::InputArchive& operator>>(FishEngine::InputArchive& archive, FishEditor::ModelImporterClipAnimation& t);
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const FishEditor::ModelImporterClipAnimation& t);
	
	// ModelImporterAnimation
	FishEngine::InputArchive& operator>>(FishEngine::InputArchive& archive, FishEditor::ModelImporterAnimation& t);
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const FishEditor::ModelImporterAnimation& t);
	
	// ModelImporterMesh
	FishEngine::InputArchive& operator>>(FishEngine::InputArchive& archive, FishEditor::ModelImporterMesh& t);
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const FishEditor::ModelImporterMesh& t);
}

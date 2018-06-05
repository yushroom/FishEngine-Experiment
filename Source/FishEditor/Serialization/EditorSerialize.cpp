#include <FishEditor/Serialization/EditorSerialize.hpp>

#include <FishEngine/Serialization/Archive.hpp>

#include <FishEditor/AssetImporter.hpp>
#include <FishEditor/ModelImporter.hpp>
#include <FishEditor/Serialization/DefaultImporter.hpp>
#include <FishEditor/Serialization/NativeFormatImporter.hpp>

using namespace FishEngine;
using namespace FishEditor;

namespace FishEditor
{
	
// ModelImporterClipAnimation
InputArchive& operator>>(InputArchive& archive, ModelImporterClipAnimation& t)
{
	archive.AddNVP("name", t.name);
	archive.AddNVP("takeName", t.takeName);
	archive.AddNVP("firstFrame", t.firstFrame);
	archive.AddNVP("lastFrame", t.lastFrame);
	return archive;
}
OutputArchive& operator<<(OutputArchive& archive, const ModelImporterClipAnimation& t)
{
	archive.AddNVP("name", t.name);
	archive.AddNVP("takeName", t.takeName);
	archive.AddNVP("firstFrame", t.firstFrame);
	archive.AddNVP("lastFrame", t.lastFrame);
	return archive;
}

// ModelImporterAnimation
InputArchive& operator>>(InputArchive& archive, ModelImporterAnimation& t)
{
	archive.AddNVP("clipAnimations", t.clipAnimations);
	archive.AddNVP("isReadable", t.isReadable);
	return archive;
}
OutputArchive& operator<<(OutputArchive& archive, const ModelImporterAnimation& t)
{
	archive.AddNVP("clipAnimations", t.clipAnimations);
	archive.AddNVP("isReadable", t.isReadable);
	return archive;
}

// ModelImporterMesh
InputArchive& operator>>(InputArchive& archive, ModelImporterMesh& t)
{
	archive.AddNVP("globalScale", t.globalScale);
	archive.AddNVP("addColliders", t.addColliders);
	archive.AddNVP("importBlendShapes", t.importBlendShapes);
	archive.AddNVP("swapUVChannels", t.swapUVChannels);
	archive.AddNVP("useFileUnits", t.useFileUnits);
	archive.AddNVP("useFileScale", t.useFileScale);
	return archive;
}
OutputArchive& operator<<(OutputArchive& archive, const ModelImporterMesh& t)
{
	archive.AddNVP("globalScale", t.globalScale);
	archive.AddNVP("addColliders", t.addColliders);
	archive.AddNVP("importBlendShapes", t.importBlendShapes);
	archive.AddNVP("swapUVChannels", t.swapUVChannels);
	archive.AddNVP("useFileUnits", t.useFileUnits);
	archive.AddNVP("useFileScale", t.useFileScale);
	return archive;
}
}

// AssetImoprter
void AssetImporter::Deserialize(InputArchive& archive)
{
	Object::Deserialize(archive);
}

void AssetImporter::Serialize(OutputArchive& archive) const
{
	Object::Serialize(archive);
}

// ModelImporter
void ModelImporter::Deserialize(InputArchive& archive)
{
	AssetImporter::Deserialize(archive);
	archive.AddNVP("fileIDToRecycleName", this->m_FileIDToRecycleName);
	archive.AddNVP("animations", this->animations);
	archive.AddNVP("meshes", this->meshes);
	archive.AddNVP("importAnimation", this->importAnimation);
	archive.AddNVP("humanDescription", this->humanDescription);
	archive.AddNVP("animationType", this->animationType);
}

void ModelImporter::Serialize(OutputArchive& archive) const
{
	AssetImporter::Serialize(archive);
	archive.AddNVP("fileIDToRecycleName", this->m_FileIDToRecycleName);
	archive.AddNVP("animations", this->animations);
	archive.AddNVP("meshes", this->meshes);
	archive.AddNVP("importAnimation", this->importAnimation);
	archive.AddNVP("humanDescription", this->humanDescription);
	archive.AddNVP("animationType", this->animationType);
}

// DefaultImporter
void DefaultImporter::Deserialize(InputArchive& archive)
{
	AssetImporter::Deserialize(archive);
}

void DefaultImporter::Serialize(OutputArchive& archive) const
{
	AssetImporter::Serialize(archive);
}


// NativeFormatImporter
void NativeFormatImporter::Deserialize(InputArchive& archive)
{
	AssetImporter::Deserialize(archive);
	archive.AddNVP("mainObjectFileID", this->m_MainObjectFileID);
}

void NativeFormatImporter::Serialize(OutputArchive& archive) const
{
	AssetImporter::Serialize(archive);
	archive.AddNVP("mainObjectFileID", this->m_MainObjectFileID);
}


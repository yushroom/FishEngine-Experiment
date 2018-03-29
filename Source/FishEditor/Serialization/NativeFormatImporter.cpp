#include <FishEditor/Serialization/NativeFormatImporter.hpp>
#include <FishEditor/Serialization/YAMLArchive.hpp>
#include <FishEditor/Path.hpp>

using namespace FishEngine;

namespace FishEditor
{
    FishEngine::Object* NativeFormatImporter::Import()
	{
		YAMLInputArchive archive;
        auto ext = fs::path(m_AssetPath).extension();
        if (ext == ".prefab")
        {

        }
		return nullptr;
	}
}

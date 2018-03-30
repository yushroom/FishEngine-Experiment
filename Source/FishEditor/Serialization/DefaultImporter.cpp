#include <FishEditor/Serialization/DefaultImporter.hpp>
#include <FishEditor/Serialization/YAMLArchive.hpp>
#include <FishEngine/Scene.hpp>

#include <FishEditor/Path.hpp>

using namespace FishEngine;

namespace FishEditor
{
	void DefaultImporter::Import()
	{
		auto fullpath = this->GetFullPath();
		assert(fs::exists(fullpath));
		auto sceneName = fs::path(fullpath).stem().string();

		Scene* old = SceneManager::GetActiveScene();
		Scene* scene = SceneManager::CreateScene(sceneName);
		YAMLInputArchive archive;
		auto objects = archive.LoadAll(fullpath);
		SceneManager::SetActiveScene(old);
	}
}
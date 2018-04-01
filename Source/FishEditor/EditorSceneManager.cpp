#include <FishEditor/EditorSceneManager.hpp>
#include <FishEditor/Serialization/DefaultImporter.hpp>

namespace FishEditor
{
	FishEngine::Scene* EditorSceneManager::OpenScene(const std::string& scenePath, OpenSceneMode mode/* = OpenSceneMode::Single*/)
	{
		auto importer = dynamic_cast<DefaultImporter*>( AssetImporter::GetAtPath(scenePath) );
		return importer->GetScene();
	}
}

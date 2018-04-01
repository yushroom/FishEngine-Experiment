#include <FishEditor/Serialization/DefaultImporter.hpp>
#include <FishEditor/Serialization/YAMLArchive.hpp>
#include <FishEngine/Scene.hpp>

#include <FishEditor/Path.hpp>
#include <algorithm>

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
		SceneManager::SetActiveScene(scene);
		YAMLInputArchive archive;
		std::string str = ReadFileAsString(fullpath);
		auto objects = archive.LoadAllFromString(str);
		SceneManager::SetActiveScene(old);

//		auto&& transforms = m_Scene->FindComponents<Transform>();
		std::vector<Transform*> transforms;
		for (auto o : objects) {
			if (o != nullptr && o->GetClassID() == Transform::ClassID) {
				auto t = o->As<Transform>();
				if (t->GetParent() == nullptr)
					transforms.push_back(o->As<Transform>());
			}
		}
		sort(transforms.begin(), transforms.end(), [](Transform* a, Transform* b){
			return a->GetRootOrder() < b->GetRootOrder();
		});

		for (auto t : transforms)
			scene->AddRootTransform(t);

		m_Scene = scene;
	}
}

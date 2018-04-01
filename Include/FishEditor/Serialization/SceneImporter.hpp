#pragma once

#include "YAMLArchive.hpp"
#include "../AssetImporter.hpp"

#include <FishEngine/Scene.hpp>
#include "../Path.hpp"

namespace FishEditor
{
	class SceneInputArchive : public AssetImporter
	{
	public:
		SceneInputArchive(const std::string& path)
			: archive(path)
		{
			m_AssetPath = path;
		}

		Scene* Import()
		{
			fs::path path = m_AssetPath;
			auto fn = path.stem().string();
			auto old = SceneManager::GetActiveScene();
			auto scene = SceneManager::CreateScene(fn);
			SceneManager::SetActiveScene(scene);
			archive.LoadAllFromString();
			SceneManager::SetActiveScene(old);
			return scene;
		}

	private:
		YAMLInputArchive archive;
	};
}

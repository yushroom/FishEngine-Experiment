#pragma once

#include "../AssetImporter.hpp"
#include <FishEngine/Scene.hpp>

namespace FishEditor
{
	class DefaultAsset
	{
		InjectClassName(DefaultAsset, 1029);
	};

	// scene importer, supported ext: .unity
	class DefaultImporter : public AssetImporter
	{
	public:
		InjectClassName(DefaultImporter, 1030);

		DefaultImporter() : AssetImporter(ClassID, ClassName)
		{
		}

		virtual void Import() override;
	
		FishEngine::Scene* GetScene() const
		{
			return m_Scene;
		}
		
	private:
		FishEngine::Scene*	m_Scene = nullptr;
	};
}

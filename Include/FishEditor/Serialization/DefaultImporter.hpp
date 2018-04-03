#pragma once

#include "../AssetImporter.hpp"
#include <FishEngine/Scene.hpp>

namespace FishEditor
{
	class DefaultAsset : public FishEngine::Object
	{
		InjectClassName(DefaultAsset, 1029);
		DefaultAsset() : Object(ClassID, ClassName) {

		}
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

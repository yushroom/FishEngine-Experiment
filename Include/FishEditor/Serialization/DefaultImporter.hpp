#pragma once

#include "../AssetImporter.hpp"

namespace FishEditor
{
	class DefaultAsset
	{
		InjectClassName(DefaultAsset, 1029);
	};

	class DefaultImporter : public AssetImporter
	{
	public:
		InjectClassName(DefaultImporter, 1030);

		DefaultImporter() : AssetImporter(ClassID, ClassName)
		{
		}

		virtual void Import() override;
	};
}
#pragma once

#include "../AssetImporter.hpp"
#include <FishEngine/Prefab.hpp>

namespace FishEditor
{
	// supported ext: .prefab, .mat
	class NativeFormatImporter : public AssetImporter
	{
	public:
		InjectClassName(NativeFormatImporter, 1034);

		NativeFormatImporter() : AssetImporter(ClassID, ClassName)
		{
		}

		virtual void Import() override;
	};
}

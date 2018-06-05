#pragma once

#include "../AssetImporter.hpp"
#include <FishEngine/Prefab.hpp>

namespace FishEditor
{
	// supported ext: .prefab, .mat
	class NativeFormatImporter : public AssetImporter
	{
	public:
		DeclareObject(NativeFormatImporter, 1034);

		NativeFormatImporter() : AssetImporter(ClassID, ClassName)
		{
		}

		virtual void Import() override;
		
	protected:
		int64_t m_MainObjectFileID = 0;
	};
}

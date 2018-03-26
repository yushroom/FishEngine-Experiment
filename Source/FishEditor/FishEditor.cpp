#include <FishEditor/FishEditor.hpp>

#include <FishEditor/AssetImporter.hpp>

namespace FishEditor
{
	void Clean()
	{
		for (auto&& p : AssetImporter::GetGUIDToImporter())
		{
			delete p.second;
		}
	}
}
#pragma once

#include <FishEngine/Object.hpp>

namespace FishEditor
{
	class AssetImporter
	{
	public:
		AssetImporter() = default;
		virtual ~AssetImporter() = default;

		// noncopyable
		AssetImporter(AssetImporter const &) = delete;
		AssetImporter& operator=(AssetImporter const &) = delete;
		
		void SaveAndReimport();
	};
}

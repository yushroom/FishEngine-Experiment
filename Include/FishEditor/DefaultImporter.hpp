#pragma once

#include "AssetImporter.hpp"

namespace FishEditor
{
	class DefaultAsset
	{
	public:
		enum { ClassID = 1029 };
	};

	class SceneAsset
	{
	public:
		enum { ClassID = 1032 };
	};

	class DefaultImporter : public AssetImporter
	{
	public:
		enum { ClassID = 1030 };
		DefaultImporter() : AssetImporter(ClassID) { }
	};
}
#pragma once

#include "FileNode.hpp"
#include <unordered_map>

#include <FishEngine/Object.hpp>


namespace FishEditor
{
	class AssetImporter;
	
	class AssetDatabase
	{
	public:
		static FileNode* AssetRootDir()
		{
			return s_AssetRootDir;
		}

		static void StaticInit();
		static void StaticClean();



		// Returns the main asset object at assetPath.
		// All paths are relative to the project folder, for example: "Assets/MyTextures/hello.png".
		static FishEngine::Object* LoadMainAssetAtPath(const std::string& path);

		static bool IsMainAsset(FishEngine::Object* obj);
		static bool IsMainAsset(int instanceID);

		static std::string AssetPathToGUID(const std::string& path);
		static std::string GUIDToAssetPath(const std::string& guid);

		static std::string GetAssetPathFromInstanceID(int instanceID);
		static std::string GetGUIDFromInstanceID(int instanceID);


		static void AddAssetPathAndGUIDPair(const std::string& path, const std::string& guid);

		static std::string GetAssetRootDir() { return s_AssetRootDir->path.string(); }

		static FishEngine::Object* GetAssetByGUIDAndFileID(const std::string& guid, int64_t fileID);

	private:
		friend class EditorApplication;
		//friend class FileNode;
		//friend class AssetImporter;
		static FileNode* s_AssetRootDir;

		static std::unordered_map<std::string, std::string> s_PathToGUID;
		static std::unordered_map<std::string, std::string> s_GUIDToPath;
		static std::unordered_map<std::string, AssetImporter*> s_GUIDToImporter;
		static std::unordered_map<int, AssetImporter*> s_AssetInstanceIDToImporter;
	};
}

#pragma once

#include "FileNode.hpp"
#include <unordered_map>

namespace FishEditor
{
    class AssetDatabase
    {
    public:
        static FileNode* AssetRootDir()
        {
            return s_AssetRootDir;
        }

		static std::string AssetPathToGUID(const std::string& path);
		static std::string GUIDToAssetPath(const std::string& guid);

    private:
        friend class EditorApplication;
		friend class FileNode;
		friend class AssetImporter;
        static FileNode* s_AssetRootDir;

		static std::unordered_map<std::string, std::string> s_PathToGUID;
		static std::unordered_map<std::string, std::string> s_GUIDToPath;
    };
}
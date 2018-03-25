#pragma once

#include "FileNode.hpp"

namespace FishEditor
{
    class AssetDatabase
    {
    public:
        static FileNode* AssetRootDir()
        {
            return s_AssetRootDir;
        }

    private:
        friend class EditorApplication;
        static FileNode* s_AssetRootDir;
    };
}
#pragma once

#include <iostream>
#include <FishGUI/Utils.hpp>
#include "Path.hpp"


namespace FishEditor
{
	class FileNode
	{
	public:
		FileNode() = default;
		FileNode(const FileNode&) = delete;
		FileNode& operator=(FileNode&) = delete;

		Path 					path;
		std::string 			fileName;
		FileNode* 				parent = nullptr;
		bool 					isDir = false;
		std::vector<FileNode*> 	subdirs;
		std::vector<FileNode*> 	files;
		std::string				guid;
		
		FileNode(const Path& rootDir);
		
		// FileNode* Find(const Path& p)
		// {
		// 	auto r = fs::relative(p, path);
		// 	std::cout << r << std::endl;
		// 	std::cout << r.root_directory() << std::endl;
		// 	return nullptr;
		// }
	};
}

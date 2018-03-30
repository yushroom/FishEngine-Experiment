#pragma once

#include <string>

namespace FishEngine
{
	std::string GetNameByClassID(int classID);
	int GetClassIDByClassName(const std::string& className);
}

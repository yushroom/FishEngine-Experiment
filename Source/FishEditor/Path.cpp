#include <FishEditor/Path.hpp>
#include <fstream>
#include <sstream>

namespace FishEditor
{
	std::string ReadFileAsString(const std::string &path)
	{
//		fs::path p(path);
		assert(fs::exists(path));
		std::ifstream is(path);
		std::stringstream buffer;
		buffer << is.rdbuf();
		return buffer.str();
	}
}
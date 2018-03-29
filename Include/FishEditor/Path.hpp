#pragma once

#ifdef _WIN32
#include <filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
namespace fs = boost::filesystem;
#endif
typedef fs::path Path;
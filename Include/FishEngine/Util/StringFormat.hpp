#pragma once
#include "../FishEngine.hpp"
#include <folly/Format.h>

namespace FishEngine
{
	template<class... TOther>
	std::string Format(const char* format, TOther&&... other)
	{
		return folly::sformat(format, other...);
	}
}

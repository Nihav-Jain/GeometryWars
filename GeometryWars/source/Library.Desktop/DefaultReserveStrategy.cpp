#include "pch.h"
#include "DefaultReserveStrategy.h"

namespace Library
{
	std::uint32_t DefaultReserveStrategy::operator()(std::uint32_t size, std::uint32_t capacity) const
	{
		UNREFERENCED_PARAMETER(size);
		return 2 * capacity + 1;
	}
}
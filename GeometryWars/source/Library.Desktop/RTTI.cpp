#include "pch.h"
#include "RTTI.h"

namespace Library
{
	Hashmap<std::uint64_t const*, std::uint64_t const*> RTTI::ClassHeirarchy(0);
	const std::uint64_t RTTI::sRunTimeTypeId = reinterpret_cast<std::uint64_t>(&RTTI::sRunTimeTypeId);
}

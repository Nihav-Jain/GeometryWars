#include "pch.h"
#include "RTTI.h"

namespace Library
{
	Hashmap<std::uint64_t const*, std::uint64_t const*> RTTI::ClassHeirarchy;
	const std::uint64_t RTTI::sRunTimeTypeId = reinterpret_cast<std::uint64_t>(&RTTI::sRunTimeTypeId);
}

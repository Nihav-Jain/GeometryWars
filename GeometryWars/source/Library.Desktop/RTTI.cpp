#include "pch.h"
#include "RTTI.h"

namespace Library
{
	Vector<RTTIData> RTTI::ClassHeirarchy(0U);
	const std::uint64_t RTTI::sRunTimeTypeId = reinterpret_cast<std::uint64_t>(&RTTI::sRunTimeTypeId);
}

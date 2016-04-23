#include "pch.h"
#include "RTTI.h"

namespace Library
{
	Graph<std::uint64_t> RTTI::ClassHeirarchy;
	Graph<std::uint64_t>::Traversor RTTI::RootTraversor = RTTI::ClassHeirarchy.AddVertex(0U);
}

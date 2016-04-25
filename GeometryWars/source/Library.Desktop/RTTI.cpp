#include "pch.h"
#include "RTTI.h"

namespace Library
{
	const std::uint64_t RTTI::sRunTimeTypeId = reinterpret_cast<std::uint64_t>(&RTTI::sRunTimeTypeId);
	Graph<const std::uint64_t*>::Traversor RTTI::ParentItr = RTTI::ClassHeirarchy().AddVertex(nullptr);
	const RTTI::RTTIClassHeirarchyDeleter RTTI::sDeleter;

	Graph<const std::uint64_t*>& RTTI::ClassHeirarchy()
	{
		static Graph<const std::uint64_t*>* map = new Graph<const std::uint64_t*>();
		return *map;
	}

	RTTI::RTTIClassHeirarchyDeleter::RTTIClassHeirarchyDeleter() :
		// this ensures that the static hashmap is constructed before the completetion of the constrcutor of RTTIClassHeirarchyDeleter
		// which, in turn, ensures that the static ClassHeirarchy hashmap ptr will not be deleted before the destructor of RTTIClassHeirarchyDeleter
		mJustAPtr(&RTTI::ClassHeirarchy())	
	{}

	RTTI::RTTIClassHeirarchyDeleter::~RTTIClassHeirarchyDeleter()
	{
		delete &RTTI::ClassHeirarchy();
	}
}

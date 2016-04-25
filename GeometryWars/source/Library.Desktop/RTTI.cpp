#include "pch.h"
#include "RTTI.h"

namespace Library
{
	const std::uint64_t RTTI::sRunTimeTypeId = reinterpret_cast<std::uint64_t>(&RTTI::sRunTimeTypeId);
	const RTTI::RTTIClassHeirarchyDeleter RTTI::sDeleter;

	Hashmap<std::uint64_t, const std::uint64_t*>& RTTI::ClassHeirarchy()
	{
		static Hashmap<std::uint64_t, const std::uint64_t*>* map = new Hashmap<std::uint64_t, const std::uint64_t*>();
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

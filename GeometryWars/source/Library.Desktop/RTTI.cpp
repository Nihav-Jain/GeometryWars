#include "pch.h"
#include "RTTI.h"

namespace Library
{
	Hashmap<std::string, std::string>* RTTI::ClassHeirarchyPtr = &RTTI::ClassHeirarchy();
	const std::uint64_t RTTI::sRunTimeTypeId = reinterpret_cast<std::uint64_t>(&RTTI::sRunTimeTypeId);
	const RTTI::RTTIClassHeirarchyDeleter RTTI::sDeleter;

	Hashmap<std::string, std::string>& RTTI::ClassHeirarchy()
	{
		static Hashmap<std::string, std::string>* map = new Hashmap<std::string, std::string>();
		return *map;
	}

	RTTI::RTTIClassHeirarchyDeleter::~RTTIClassHeirarchyDeleter()
	{
		delete &RTTI::ClassHeirarchy();
	}
}

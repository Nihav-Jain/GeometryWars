#include "pch.h"
#include "Action.h"

namespace Library
{
	RTTI_DEFINITIONS(Action);

	const std::uint32_t Action::NUM_RESERVED_PRESCRIBED_ATTRIBUTES = 2;
	const std::string Action::ATTRIBUTE_NAME = "name";
	
	Action::Action() :
		mName()
	{
		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
	}

	const std::string& Action::Name() const
	{
		return mName;
	}

	void Action::SetName(const std::string& name)
	{
		mName = name;
	}

	void Action::PostParsingProcess()
	{}

}

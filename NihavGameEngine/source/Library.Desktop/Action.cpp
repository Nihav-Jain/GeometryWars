#include "pch.h"
#include "Action.h"

namespace Library
{
	RTTI_DEFINITIONS(Action);

	const std::string Action::ATTRIBUTE_NAME = "name";
	
	Action::Action() :
		mName()
	{
		Populate();
	}

	Action::~Action()
	{
	}

	Action::Action(const Action& rhs) :
		mName(rhs.mName), Attributed(rhs)
	{
		(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
	}

	Action::Action(Action&& rhs) :
		mName(std::move(rhs.mName)), Attributed(std::move(rhs))
	{
		(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
	}

	Action& Action::operator=(const Action& rhs)
	{
		if (this != &rhs)
		{
			mName = rhs.mName;
			Attributed::operator=(rhs);

			(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
		}
		return *this;
	}

	Action& Action::operator=(Action&& rhs)
	{
		if (this != &rhs)
		{
			mName = std::move(rhs.mName);
			Attributed::operator=(std::move(rhs));

			(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
		}

		return *this;
	}

	const std::string& Action::Name() const
	{
		return mName;
	}

	void Action::SetName(const std::string& name)
	{
		mName = name;
	}

	void Action::DefinePrescribedAttributes()
	{
		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
	}

}

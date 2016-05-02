#include "pch.h"
#include "Reaction.h"

namespace Library
{
	RTTI_DEFINITIONS(Reaction, ActionList);

	Reaction::Reaction()
	{}

	Reaction::Reaction(const Reaction& rhs) :
		ActionList::ActionList(rhs)
	{}

	Reaction::Reaction(Reaction&& rhs) :
		ActionList::ActionList(std::move(rhs))
	{}

	Reaction& Reaction::operator=(const Reaction& rhs)
	{
		ActionList::operator=(rhs);
		return *this;
	}

	Reaction& Reaction::operator=(Reaction&& rhs)
	{
		ActionList::operator=(std::move(rhs));
		return *this;
	}

	void Reaction::Update(WorldState& worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
	}
}

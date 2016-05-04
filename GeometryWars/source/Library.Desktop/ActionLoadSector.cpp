#include "pch.h"
#include "ActionLoadSector.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionLoadSector, Action);

	const std::string ActionLoadSector::ATTRIBUTE_SECTOR_NAME = "sectorName";

	ActionLoadSector::ActionLoadSector()
	{
		AddInternalAttribute(ATTRIBUTE_SECTOR_NAME, "", 0);
	}

	ActionLoadSector::ActionLoadSector(const ActionLoadSector& rhs) :
		Action::Action(rhs)
	{}

	ActionLoadSector::ActionLoadSector(ActionLoadSector&& rhs) :
		Action::Action(std::move(rhs))
	{}

	ActionLoadSector& ActionLoadSector::operator=(const ActionLoadSector& rhs)
	{
		Action::operator=(rhs);
		return *this;
	}

	ActionLoadSector& ActionLoadSector::operator=(ActionLoadSector&& rhs)
	{
		Action::operator=(std::move(rhs));
		return *this;
	}

	void ActionLoadSector::Update(WorldState& worldState)
	{
		bool willLoadNewSector = worldState.world->LoadSector((*this)[ATTRIBUTE_SECTOR_NAME].Get<std::string>());
		if (!willLoadNewSector)
		{
			// log error and move on
		}
	}

	Scope* ActionLoadSector::Clone(const Scope& rhs) const
	{
		ActionLoadSector& action = *rhs.AssertiveAs<ActionLoadSector>();
		return new ActionLoadSector(action);
	}

}

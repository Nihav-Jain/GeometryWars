#include "pch.h"
#include "ActionList.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionList, Action);

	const std::string ActionList::ATTRIBUTE_ACTIONS = "actions";

	ActionList::ActionList()
	{
		AddNestedScope(ATTRIBUTE_ACTIONS);
	}

	Datum& ActionList::Actions()
	{
		return operator[](ATTRIBUTE_ACTIONS);
	}

	const Datum& ActionList::Actions() const
	{
		return *Find(ATTRIBUTE_ACTIONS);
	}

	Action* ActionList::FindAction(const std::string& actionName) const
	{
		return Action::FindAction(actionName, Actions());
	}

	void ActionList::Update(WorldState& worldState)
	{
		Datum& actions = Actions();
		std::uint32_t i;
		for (i = 0; i < actions.Size(); i++)
		{
			Action* action = actions.Get<Scope>(i).As<Action>();
			assert(action != nullptr);
			worldState.action = action;
			action->Update(worldState);
		}
	}
}

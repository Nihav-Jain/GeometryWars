#include "pch.h"
#include "ActionList.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionList);

	const std::string ActionList::ATTRIBUTE_ACTIONS = "actions";

	ActionList::ActionList()
	{
		Populate();
	}

	Datum& ActionList::Actions()
	{
		return operator[](ATTRIBUTE_ACTIONS);
	}

	const Datum& ActionList::Actions() const
	{
		return *Find(ATTRIBUTE_ACTIONS);
	}

	Action& ActionList::CreateAction(const std::string& actionClassName, const std::string& actionInstanceName)
	{
		Action* action = Factory<Action>::Create(actionClassName);
		assert(action != nullptr);
		action->SetName(actionInstanceName);

		Adopt(ATTRIBUTE_ACTIONS, *action);

		return *action;
	}

	Action* ActionList::FindAction(const std::string& actionName) const
	{
		const Datum& actions = Actions();
		for (std::uint32_t i = 0; i < actions.Size(); i++)
		{
			Action* action = actions.Get<Scope*>(i)->As<Action>();
			assert(action != nullptr);
			if (action->Name() == actionName)
				return action;
		}
		return nullptr;
	}

	void ActionList::Update(WorldState& worldState)
	{
		Datum& actions = Actions();
		std::uint32_t i;
		for (i = 0; i < actions.Size(); i++)
		{
			Action* action = actions.Get<Scope*>(i)->As<Action>();
			assert(action != nullptr);
			worldState.action = action;
			action->Update(worldState);
		}
	}

	void ActionList::DefinePrescribedAttributes()
	{
		AddNestedScope(ATTRIBUTE_ACTIONS, 0);
	}
}

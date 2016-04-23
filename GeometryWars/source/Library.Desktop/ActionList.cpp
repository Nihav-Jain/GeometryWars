#include "pch.h"
#include "ActionList.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionList);

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

	void ActionList::BeginPlay(WorldState& worldState)
	{
		worldState.action = this;

		ScriptedBeginPlay(worldState);
		ActionsBeginPlay(worldState);
		ReactionsBeginPlay(worldState);
	}

	void ActionList::Update(WorldState& worldState)
	{
		Datum& actions = Actions();
		std::uint32_t i;
		for (i = 0; i < actions.Size(); i++)
		{
			Action* action = actions.Get<Scope>(i).AssertiveAs<Action>();
			if ((*action)[Action::ATTRIBUTE_CAN_EVER_TICK].Get<bool>())
			{
				worldState.action = action;
				action->Update(worldState);
			}
		}
	}

	void ActionList::ScriptedBeginPlay(WorldState& worldState)
	{
		Datum* beginPlayDatum = Find(World::ATTRIBUTE_BEGIN_PLAY);
		if (beginPlayDatum != nullptr && beginPlayDatum->Size() > 0)
		{
			ActionList* beginPlayList = beginPlayDatum->Get<Scope>().AssertiveAs<ActionList>();
			beginPlayList->BeginPlay(worldState);
			beginPlayList->Update(worldState);
		}
	}

	void ActionList::ActionsBeginPlay(WorldState& worldState)
	{
		std::uint32_t i;
		Datum& actions = Actions();
		for (i = 0; i < actions.Size(); i++)
		{
			actions.Get<Scope>(i).AssertiveAs<Action>()->BeginPlay(worldState);
		}
	}

	void ActionList::ReactionsBeginPlay(WorldState& worldState)
	{
		std::uint32_t i;
		Datum* reactions = Find(World::ATTRIBUTE_REACTIONS);
		if (reactions != nullptr)
		{
			for (i = 0; i < reactions->Size(); i++)
			{
				reactions->Get<Scope>(i).AssertiveAs<Action>()->BeginPlay(worldState);
			}
		}
	}
}

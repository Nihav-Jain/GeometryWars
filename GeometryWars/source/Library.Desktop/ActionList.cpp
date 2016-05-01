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

	ActionList::ActionList(const ActionList& rhs) :
		Action::Action(rhs)
	{}

	ActionList::ActionList(ActionList&& rhs) :
		Action::Action(std::move(rhs))
	{}

	ActionList& ActionList::operator=(const ActionList& rhs)
	{
		Action::operator=(rhs);
		return *this;
	}

	ActionList& ActionList::operator=(ActionList&& rhs)
	{
		Action::operator=(std::move(rhs));
		return *this;
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
		Action::BeginPlay(worldState);

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

	void ActionList::OnDestroy(WorldState& worldState)
	{
		Action::OnDestroy(worldState);

		ScriptedOnDestroy(worldState);
		ActionsOnDestroy(worldState);
		ReactionsOnDestroy(worldState);
	}

	Scope* ActionList::Clone(const Scope& rhs) const
	{
		ActionList& actionList = *rhs.AssertiveAs<ActionList>();
		return new ActionList(actionList);
	}

	void ActionList::ScriptedBeginPlay(WorldState& worldState)
	{
		Datum* beginPlayDatum = Find(World::ATTRIBUTE_BEGIN_PLAY);
		if (beginPlayDatum != nullptr && beginPlayDatum->Size() > 0)
		{
			ActionList* beginPlayList = beginPlayDatum->Get<Scope>().AssertiveAs<ActionList>();
			worldState.action = beginPlayList;
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
			Action* action = actions.Get<Scope>(i).AssertiveAs<Action>();
			worldState.action = action;
			action->BeginPlay(worldState);
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
				Action* reaction = reactions->Get<Scope>(i).AssertiveAs<Action>();
				worldState.action = reaction;
				reaction->BeginPlay(worldState);
			}
		}
	}

	void ActionList::ScriptedOnDestroy(WorldState& worldState)
	{
		Datum* onDestroyDatum = Find(World::ATTRIBUTE_ON_DESTROY);
		if (onDestroyDatum != nullptr && onDestroyDatum->Size() > 0)
		{
			ActionList* onDestroyList = onDestroyDatum->Get<Scope>().AssertiveAs<ActionList>();
			worldState.action = onDestroyList;
			onDestroyList->OnDestroy(worldState);
		}
	}

	void ActionList::ActionsOnDestroy(WorldState& worldState)
	{
		std::uint32_t i;
		Datum& actions = Actions();
		for (i = 0; i < actions.Size(); i++)
		{
			Action* action = actions.Get<Scope>(i).AssertiveAs<Action>();
			worldState.action = action;
			action->OnDestroy(worldState);
		}
	}

	void ActionList::ReactionsOnDestroy(WorldState& worldState)
	{
		std::uint32_t i;
		Datum* reactions = Find(World::ATTRIBUTE_REACTIONS);
		if (reactions != nullptr)
		{
			for (i = 0; i < reactions->Size(); i++)
			{
				Action* reaction = reactions->Get<Scope>(i).AssertiveAs<Action>();
				worldState.action = reaction;
				reaction->OnDestroy(worldState);
			}
		}
	}
}

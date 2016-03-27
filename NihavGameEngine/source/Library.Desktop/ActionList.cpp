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

	ActionList::ActionList(const ActionList& rhs) :
		Action(rhs)
	{}

	ActionList::ActionList(ActionList&& rhs) :
		Action(std::move(rhs))
	{}


	ActionList::~ActionList()
	{
	}

	ActionList& ActionList::operator=(const ActionList& rhs)
	{
		if (this != &rhs)
		{
			Action::operator=(rhs);
		}
		return *this;
	}

	ActionList& ActionList::operator=(ActionList&& rhs)
	{
		if (this != &rhs)
		{
			Action::operator=(std::move(rhs));
		}
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
	
	Scope* ActionList::Clone(const Scope& rhs) const
	{
		if (!rhs.Is(ActionList::TypeIdClass()))
			throw std::exception("Given Scope reference is not an ActionList.");
		return new ActionList(*(rhs.As<ActionList>()));
	}

	void ActionList::DefinePrescribedAttributes()
	{
		AddNestedScope(ATTRIBUTE_ACTIONS, Scope(), 0);
	}
}

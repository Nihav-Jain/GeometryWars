#include "pch.h"
#include "Action.h"

namespace Library
{
	RTTI_DEFINITIONS(Action);

	const std::uint32_t Action::NUM_RESERVED_PRESCRIBED_ATTRIBUTES = 3;
	const std::string Action::ATTRIBUTE_NAME = "name";
	const std::string Action::ATTRIBUTE_CAN_EVER_TICK = "canEverTick";
	
	Action::Action() :
		mName()
	{
		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
		AddInternalAttribute(ATTRIBUTE_CAN_EVER_TICK, true, 1);
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

	Action* Action::FindAction(const std::string& actionName, const Datum& actions)
	{
		for (std::uint32_t i = 0; i < actions.Size(); i++)
		{
			Action* action = actions.Get<Scope>(i).As<Action>();
			assert(action != nullptr);
			if (action->Name() == actionName)
				return action;
		}
		return nullptr;
	}

	Action& Action::CreateAction(const std::string& actionClassName, const std::string& actionInstanceName, Scope& parentScope, const std::string& attributeName)
	{
		Action* action = Factory<Action>::Create(actionClassName);
		assert(action != nullptr);
		action->SetName(actionInstanceName);

		parentScope.Adopt(attributeName, *action);

		return *action;
	}

}

#include "pch.h"
#include "ActionCreateAction.h"
#include "ActionList.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionCreateAction);

	const std::string ActionCreateAction::ATTRIBUTE_PROTOTYPE_CLASS = "prototypeClass";
	const std::string ActionCreateAction::ATTRIBUTE_PROTOTYPE_NAME = "prototypeName";

	ActionCreateAction::ActionCreateAction()
	{
		Populate();
	}

	void ActionCreateAction::Update(WorldState& worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
		if (GetParent()->Is(Entity::TypeIdClass()))
		{
			Entity* entity = GetParent()->AssertiveAs<Entity>();
			entity->CreateAction((*this)[ATTRIBUTE_PROTOTYPE_CLASS].Get<std::string>(), (*this)[ATTRIBUTE_PROTOTYPE_NAME].Get<std::string>());
		}
		else if(GetParent()->Is(ActionList::TypeIdClass()))
		{
			ActionList* action = GetParent()->AssertiveAs<ActionList>();
			action->CreateAction((*this)[ATTRIBUTE_PROTOTYPE_CLASS].Get<std::string>(), (*this)[ATTRIBUTE_PROTOTYPE_NAME].Get<std::string>());
		}
	}

	void ActionCreateAction::DefinePrescribedAttributes()
	{
		AddInternalAttribute(ATTRIBUTE_PROTOTYPE_CLASS, "");
		AddInternalAttribute(ATTRIBUTE_PROTOTYPE_NAME, "");
	}
}

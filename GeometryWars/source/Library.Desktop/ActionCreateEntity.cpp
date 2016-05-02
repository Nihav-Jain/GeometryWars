#include "pch.h"
#include "ActionCreateEntity.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionCreateEntity, Action);

	const std::string ActionCreateEntity::ATTRIBUTE_ENTITY_CLASS_NAME = "entityClassName";
	const std::string ActionCreateEntity::ATTRIBUTE_ENTITY_INSTANCE_NAME = "entityInstanceName";

	ActionCreateEntity::ActionCreateEntity()
	{
		AddInternalAttribute(ATTRIBUTE_ENTITY_CLASS_NAME, "Entity", 1);
		AddInternalAttribute(ATTRIBUTE_ENTITY_INSTANCE_NAME, "entity", 1);
	}

	ActionCreateEntity::ActionCreateEntity(const ActionCreateEntity& rhs) :
		Action::Action(rhs)
	{}

	ActionCreateEntity::ActionCreateEntity(ActionCreateEntity&& rhs) :
		Action::Action(std::move(rhs))
	{}

	ActionCreateEntity& ActionCreateEntity::operator=(const ActionCreateEntity& rhs)
	{
		Action::operator=(rhs);
		return *this;
	}

	ActionCreateEntity& ActionCreateEntity::operator=(ActionCreateEntity&& rhs)
	{
		Action::operator=(std::move(rhs));
		return *this;
	}

	void ActionCreateEntity::Update(WorldState& worldState)
	{
		assert(worldState.sector != nullptr);

		Entity& newEntity = worldState.sector->CreateEntity((*this)[ATTRIBUTE_ENTITY_CLASS_NAME].Get<std::string>(), (*this)[ATTRIBUTE_ENTITY_INSTANCE_NAME].Get<std::string>());
		newEntity.BeginPlay(worldState);
	}

	Scope* ActionCreateEntity::Clone(const Scope& rhs) const
	{
		ActionCreateEntity& action = *rhs.AssertiveAs<ActionCreateEntity>();
		return new ActionCreateEntity(action);
	}

}

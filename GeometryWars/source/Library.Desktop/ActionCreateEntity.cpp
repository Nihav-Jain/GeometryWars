#include "pch.h"
#include "ActionCreateEntity.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionCreateEntity);

	const std::string ActionCreateEntity::ATTRIBUTE_ENTITY_CLASS_NAME = "entityClassName";
	const std::string ActionCreateEntity::ATTRIBUTE_ENTITY_INSTANCE_NAME = "entityInstanceName";

	ActionCreateEntity::ActionCreateEntity()
	{
		AddInternalAttribute(ATTRIBUTE_ENTITY_CLASS_NAME, "Entity", 1);
		AddInternalAttribute(ATTRIBUTE_ENTITY_INSTANCE_NAME, "entity", 1);
	}

	void ActionCreateEntity::Update(WorldState& worldState)
	{
		assert(worldState.sector != nullptr);

		Entity& newEntity = worldState.sector->CreateEntity((*this)[ATTRIBUTE_ENTITY_CLASS_NAME].Get<std::string>(), (*this)[ATTRIBUTE_ENTITY_INSTANCE_NAME].Get<std::string>());
		newEntity.BeginPlay(worldState);
	}

}

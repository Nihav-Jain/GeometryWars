#include "pch.h"
#include "ActionDestroyEntity.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionDestroyEntity);

	const std::string ActionDestroyEntity::ATTRIBUTE_ENTITY_INSTANCE_NAME = "entityInstanceName";

	ActionDestroyEntity::ActionDestroyEntity() :
		mEntityToDestroy()
	{
		AddExternalAttribute(ATTRIBUTE_ENTITY_INSTANCE_NAME, 1, &mEntityToDestroy);
	}

	void ActionDestroyEntity::Update(WorldState& worldState)
	{
		assert(worldState.sector != nullptr);

		Entity* entity = worldState.sector->FindEntity(mEntityToDestroy);
		if (entity != nullptr)
		{
			entity->MarkForDestroy(worldState);
		}
	}

}

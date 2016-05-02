#include "pch.h"
#include "ActionDestroyEntity.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionDestroyEntity, Action);

	const std::string ActionDestroyEntity::ATTRIBUTE_ENTITY_INSTANCE_NAME = "entityInstanceName";

	ActionDestroyEntity::ActionDestroyEntity() :
		mEntityToDestroy()
	{
		AddExternalAttribute(ATTRIBUTE_ENTITY_INSTANCE_NAME, 1, &mEntityToDestroy);
	}

	ActionDestroyEntity::ActionDestroyEntity(const ActionDestroyEntity& rhs) :
		Action::Action(rhs)
	{}

	ActionDestroyEntity::ActionDestroyEntity(ActionDestroyEntity&& rhs) :
		Action::Action(std::move(rhs))
	{}

	ActionDestroyEntity& ActionDestroyEntity::operator=(const ActionDestroyEntity& rhs)
	{
		Action::operator=(rhs);
		return *this;
	}

	ActionDestroyEntity& ActionDestroyEntity::operator=(ActionDestroyEntity&& rhs)
	{
		Action::operator=(std::move(rhs));
		return *this;
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

	Scope* ActionDestroyEntity::Clone(const Scope& rhs) const
	{
		ActionDestroyEntity& action = *rhs.AssertiveAs<ActionDestroyEntity>();
		return new ActionDestroyEntity(action);
	}

}

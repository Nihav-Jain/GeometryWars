#include "pch.h"
#include "ActionDestroyEntity.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionDestroyEntity, ActionList);

	const std::string ActionDestroyEntity::ATTRIBUTE_ENTITY_REFERENCE = "entityInstanceName";

	ActionDestroyEntity::ActionDestroyEntity() :
		mEntityToDestroy()
	{
		AddInternalAttribute(ATTRIBUTE_ENTITY_REFERENCE, &mEntityToDestroy, 1);
	}

	ActionDestroyEntity::~ActionDestroyEntity()
	{
		//if(mEntityToDestroy != nullptr)
		//	delete mEntityToDestroy;
	}

	ActionDestroyEntity::ActionDestroyEntity(const ActionDestroyEntity& rhs) :
		ActionList::ActionList(rhs), mEntityToDestroy(rhs.mEntityToDestroy)
	{
		(*this)[ATTRIBUTE_ENTITY_REFERENCE] = &mEntityToDestroy;
	}

	ActionDestroyEntity::ActionDestroyEntity(ActionDestroyEntity&& rhs) :
		ActionList::ActionList(std::move(rhs)), mEntityToDestroy(std::move(rhs.mEntityToDestroy))
	{
		(*this)[ATTRIBUTE_ENTITY_REFERENCE] = &mEntityToDestroy;
	}

	ActionDestroyEntity& ActionDestroyEntity::operator=(const ActionDestroyEntity& rhs)
	{
		if (this != &rhs)
		{
			mEntityToDestroy = rhs.mEntityToDestroy;
			ActionList::operator=(rhs);
			(*this)[ATTRIBUTE_ENTITY_REFERENCE] = &mEntityToDestroy;
		}
		return *this;
	}

	ActionDestroyEntity& ActionDestroyEntity::operator=(ActionDestroyEntity&& rhs)
	{
		if (this != &rhs)
		{
			mEntityToDestroy = std::move(rhs.mEntityToDestroy);
			ActionList::operator=(rhs);
			(*this)[ATTRIBUTE_ENTITY_REFERENCE] = &mEntityToDestroy;
		}
		return *this;
	}

	void ActionDestroyEntity::Update(WorldState& worldState)
	{
		ActionList::Update(worldState);
		worldState.action = this;

		assert(worldState.sector != nullptr);

		Datum& datumToDestroy = (*this)[ATTRIBUTE_ENTITY_REFERENCE].Get<Datum>();
		Entity* entity = nullptr;
		//if(datumToDestroy.Type() == Datum::DatumType::REFERENCE)
		//	entity = datumToDestroy.Get<Datum>().Get<Scope>().AssertiveAs<Entity>();
		//else if(datumToDestroy.Type() == Datum::DatumType::TABLE)
			entity = datumToDestroy.Get<Scope>().AssertiveAs<Entity>();

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

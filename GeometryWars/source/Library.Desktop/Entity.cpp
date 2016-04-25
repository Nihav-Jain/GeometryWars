#include "pch.h"
#include "Entity.h"
#include "Sector.h"
#include "Action.h"
#include "ActionDestroyEntity.h"

namespace Library
{
	RTTI_DEFINITIONS(Entity, Attributed);

	const std::uint32_t Entity::NUM_RESERVED_PRESCRIBED_ATTRIBUTES = 3;
	const std::string Entity::ATTRIBUTE_NAME = "name";
	const std::string Entity::ATTRIBUTE_ACTIONS = "actions";

	Entity::Entity() :
		mName(), mIsPendingDestroy(false)
	{
		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
		AddNestedScope(ATTRIBUTE_ACTIONS);
	}

	const std::string& Entity::Name() const
	{
		return mName;
	}

	void Entity::SetName(const std::string& name)
	{
		mName = name;
	}

	Datum& Entity::Actions()
	{
		return operator[](ATTRIBUTE_ACTIONS);
	}

	const Datum& Entity::Actions() const
	{
		return *Find(ATTRIBUTE_ACTIONS);
	}

	Action* Entity::FindAction(const std::string& actionName) const
	{
		return Action::FindAction(actionName, Actions());
	}

	Sector* Entity::GetSector() const
	{
		Scope* parent = GetParent();
		if (parent == nullptr)
			return nullptr;
		return parent->As<Sector>();
	}

	void Entity::SetSector(Sector& parent)
	{
		parent.Adopt(Sector::ATTRIBUTE_ENTITIES, *this);
	}

	void Entity::BeginPlay(WorldState& worldState)
	{
		ScriptedBeginPlay(worldState);
		ActionsBeginPlay(worldState);
		ReactionsBeginPlay(worldState);
	}

	void Entity::Update(WorldState& worldState)
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

	void Entity::OnDestroy(WorldState& worldState)
	{
		ScriptedOnDestroy(worldState);
		ActionsOnDestroy(worldState);
		ReactionsOnDestroy(worldState);
	}

	bool Entity::IsPendingDestroy() const
	{
		return mIsPendingDestroy;
	}

	void Entity::MarkForDestroy(WorldState& worldState)
	{
		if (worldState.action == nullptr || !worldState.action->Is(ActionDestroyEntity::TypeIdClass()))
			throw std::exception("Only ActionDestroyEntity can mark an entity for destory.");
		mIsPendingDestroy = true;
	}

	void Entity::ScriptedBeginPlay(WorldState& worldState)
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

	void Entity::ActionsBeginPlay(WorldState& worldState)
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

	void Entity::ReactionsBeginPlay(WorldState& worldState)
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

	void Entity::ScriptedOnDestroy(WorldState& worldState)
	{
		Datum* onDestroyDatum = Find(World::ATTRIBUTE_ON_DESTROY);
		if (onDestroyDatum != nullptr && onDestroyDatum->Size() > 0)
		{
			ActionList* onDestroyList = onDestroyDatum->Get<Scope>().AssertiveAs<ActionList>();
			worldState.action = onDestroyList;
			onDestroyList->OnDestroy(worldState);
		}
	}

	void Entity::ActionsOnDestroy(WorldState& worldState)
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

	void Entity::ReactionsOnDestroy(WorldState& worldState)
	{
		std::uint32_t i;
		Datum* reactions = Find(World::ATTRIBUTE_REACTIONS);
		if (reactions != nullptr)
		{
			for (i = 0; i < reactions->Size(); i++)
			{
				Action* reaction = reactions->Get<Scope>(i).AssertiveAs<Action>();
				reaction->OnDestroy(worldState);
			}
		}
	}

}

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
	const std::string Entity::ATTRIBUTE_OWNER_SECTOR = "ownerSector";

	Entity::Entity() :
		mName(), mIsPendingDestroy(false), mSector(new Datum())
	{
		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
		AddNestedScope(ATTRIBUTE_ACTIONS);
		AddExternalAttribute(ATTRIBUTE_OWNER_SECTOR, 1, &mSector);
	}

	Entity::~Entity()
	{
		delete mSector;
	}

	Entity::Entity(const Entity& rhs) :
		Attributed::Attributed(rhs), mName(rhs.mName), mSector(new Datum(*rhs.mSector)), mIsPendingDestroy(rhs.mIsPendingDestroy)
	{
		(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
		(*this)[ATTRIBUTE_OWNER_SECTOR].SetStorage(&mSector, 1);
	}

	Entity::Entity(Entity&& rhs) :
		Attributed::Attributed(std::move(rhs)), mName(std::move(rhs.mName)), mSector(rhs.mSector), mIsPendingDestroy(rhs.mIsPendingDestroy)
	{
		(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
		(*this)[ATTRIBUTE_OWNER_SECTOR].SetStorage(&mSector, 1);

		rhs.mSector = nullptr;
		rhs.mIsPendingDestroy = false;
	}

	Entity& Entity::operator=(const Entity& rhs)
	{
		if (this != &rhs)
		{
			delete mSector;

			mName = rhs.mName;
			mSector = new Datum(*rhs.mSector);
			mIsPendingDestroy = rhs.mIsPendingDestroy;

			Attributed::operator=(rhs);

			(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
			(*this)[ATTRIBUTE_OWNER_SECTOR].SetStorage(&mSector, 1);
		}
		return *this;
	}

	Entity& Entity::operator=(Entity&& rhs)
	{
		if (this != &rhs)
		{
			delete mSector;

			mName = std::move(rhs.mName);
			mSector = rhs.mSector;
			mIsPendingDestroy = rhs.mIsPendingDestroy;

			Attributed::operator=(std::move(rhs));

			(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
			(*this)[ATTRIBUTE_OWNER_SECTOR].SetStorage(&mSector, 1);

			rhs.mSector = nullptr;
			rhs.mIsPendingDestroy = false;
		}
		return *this;
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
		*mSector = *worldState.sector;

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

	Scope* Entity::Clone(const Scope& rhs) const
	{
		Entity& action = *rhs.AssertiveAs<Entity>();
		return new Entity(action);
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
		(worldState);
		//if (worldState.action == nullptr || !worldState.action->Is(ActionDestroyEntity::TypeIdClass()))
		//	throw std::exception("Only ActionDestroyEntity can mark an entity for destory.");
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

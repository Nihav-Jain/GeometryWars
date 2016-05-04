#include "pch.h"
#include "Sector.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(Sector, Attributed);

	const std::string Sector::ATTRIBUTE_ENTITIES = "entities";
	const std::string Sector::ATTRIBUTE_NAME = "name";
	const std::string Sector::ATTRIBUTE_OWNER_WORLD = "ownerWorld";

	Sector::Sector() :
		mName(), mEntityListByType(), mWorld(new Datum())
	{
		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
		AddNestedScope(ATTRIBUTE_ENTITIES);
		AddNestedScope(Entity::ATTRIBUTE_ACTIONS);
		AddExternalAttribute(ATTRIBUTE_OWNER_WORLD, 1, &mWorld);
	}

	Sector::~Sector()
	{
		delete mWorld;
	}
	
	Sector::Sector(const Sector& rhs) :
		Attributed::Attributed(rhs), mName(rhs.mName), mEntityListByType(rhs.mEntityListByType), mWorld(new Datum(*rhs.mWorld))
	{
		(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
		(*this)[ATTRIBUTE_OWNER_WORLD].SetStorage(&mWorld, 1);

		// TODO: update mEntityListByType
	}

	Sector::Sector(Sector&& rhs) :
		Attributed::Attributed(std::move(rhs)), mName(std::move(rhs.mName)), mEntityListByType(std::move(rhs.mEntityListByType)), mWorld(rhs.mWorld)
	{
		(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
		(*this)[ATTRIBUTE_OWNER_WORLD].SetStorage(&mWorld, 1);

		rhs.mWorld = nullptr;
	}

	Sector& Sector::operator=(const Sector& rhs)
	{
		if (this != &rhs)
		{
			delete mWorld;

			mName = rhs.mName;
			mEntityListByType = rhs.mEntityListByType;
			mWorld = new Datum(*rhs.mWorld);

			Attributed::operator=(rhs);

			(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
			(*this)[ATTRIBUTE_OWNER_WORLD].SetStorage(&mWorld, 1);

			// TODO: update mEntityListByType
		}
		return *this;
	}

	Sector& Sector::operator=(Sector&& rhs)
	{
		if (this != &rhs)
		{
			delete mWorld;

			mName = std::move(rhs.mName);
			mEntityListByType = std::move(rhs.mEntityListByType);
			mWorld = rhs.mWorld;

			Attributed::operator=(rhs);

			(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
			(*this)[ATTRIBUTE_OWNER_WORLD].SetStorage(&mWorld, 1);

			rhs.mWorld = nullptr;
		}
		return *this;
	}


	const std::string& Sector::Name() const
	{
		return mName;
	}

	void Sector::SetName(const std::string& name)
	{
		mName = name;
	}

	Datum& Sector::Entities()
	{
		return operator[](ATTRIBUTE_ENTITIES);
	}

	const Datum& Sector::Entities() const
	{
		return *Find(ATTRIBUTE_ENTITIES);
	}

	Entity& Sector::CreateEntity(const std::string& entityClassName, const std::string& entityInstanceName)
	{
		Entity* entity = Factory<Entity>::Create(entityClassName);

		assert(entity != nullptr);

		entity->SetName(entityInstanceName);
		entity->SetSector(*this);

		return *entity;
	}

	void Sector::AdoptEntity(Entity& entity, const std::string& name)
	{
		entity.SetName(name);
		entity.SetSector(*this);

		mEntityListByType[entity.TypeIdInstance()].PushBack(&entity);
		AddEntityToTypeMap(entity, RTTI::ClassHeirarchy()[entity.TypeIdInstance()]);
	}

	Entity* Sector::FindEntity(const std::string& entityName) const
	{
		const Datum& entities = Entities();
		for (std::uint32_t i = 0; i < entities.Size(); i++)
		{
			Entity* entity = entities.Get<Scope>(i).AssertiveAs<Entity>();
			if (entity->Name() == entityName)
				return entity;
		}
		return nullptr;
	}

	Vector<Entity*> Sector::FindAllEntities(const std::string& entityName) const
	{
		Vector<Entity*> listOfEntities;

		const Datum& entities = Entities();
		for (std::uint32_t i = 0; i < entities.Size(); i++)
		{
			Entity* entity = entities.Get<Scope>(i).AssertiveAs<Entity>();
			if (entity->Name() == entityName)
				listOfEntities.PushBack(entity);
		}
		return listOfEntities;
	}

	Datum& Sector::Actions()
	{
		return operator[](Entity::ATTRIBUTE_ACTIONS);
	}

	const Datum& Sector::Actions() const
	{
		return *Find(Entity::ATTRIBUTE_ACTIONS);
	}

	Action* Sector::FindAction(const std::string& actionName) const
	{
		return Action::FindAction(actionName, Actions());
	}

	World* Sector::GetWorld() const
	{
		Scope* parent = GetParent();
		if (parent == nullptr)
			return nullptr;
		return parent->As<World>();
	}

	void Sector::SetWorld(World& parent)
	{
		parent.Adopt(World::ATTRIBUTE_NAME_SECTOR, *this);
	}

	void Sector::BeginPlay(WorldState& worldState)
	{
		*mWorld = *worldState.world;

		mEntityListByType.Clear();
		Datum& entities = Entities();
		for (std::uint32_t i = 0; i < entities.Size(); i++)
		{
			Entity& entity = *entities.Get<Scope>(i).AssertiveAs<Entity>();
			mEntityListByType[entity.TypeIdInstance()].PushBack(&entity);
			AddEntityToTypeMap(entity, RTTI::ClassHeirarchy()[entity.TypeIdInstance()]);
		}


		ScriptedBeginPlay(worldState);
		EntitiesBeginPlay(worldState);
		ActionsBeginPlay(worldState);
		ReactionsBeginPlay(worldState);
	}

	void Sector::Update(WorldState& worldState)
	{
		UpdateSectorActions(worldState);
		DeletePendingDestroyEntities(worldState);
		UpdateSectorEntities(worldState);
	}

	void Sector::OnDestroy(WorldState& worldState)
	{
		ScriptedOnDestroy(worldState);
		EntitiesOnDestroy(worldState);
		ActionsOnDestroy(worldState);
		ReactionsOnDestroy(worldState);
	}

	Scope* Sector::Clone(const Scope& rhs) const
	{
		Sector& action = *rhs.AssertiveAs<Sector>();
		return new Sector(action);
	}

	const Vector<Entity*>& Sector::GetAllEntitiesOfType(std::uint64_t typeId)
	{
		return mEntityListByType[typeId];
	}

	void Sector::ScriptedBeginPlay(WorldState& worldState)
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

	void Sector::EntitiesBeginPlay(WorldState& worldState)
	{
		std::uint32_t i;
		Datum& entities = Entities();
		for (i = 0; i < entities.Size(); i++)
		{
			Entity* entity = entities.Get<Scope>(i).AssertiveAs<Entity>();
			worldState.entity = entity;
			entity->BeginPlay(worldState);
		}

		worldState.entity = nullptr;
	}

	void Sector::ActionsBeginPlay(WorldState& worldState)
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

	void Sector::ReactionsBeginPlay(WorldState& worldState)
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

	void Sector::ScriptedOnDestroy(WorldState& worldState)
	{
		Datum* onDestroyDatum = Find(World::ATTRIBUTE_ON_DESTROY);
		if (onDestroyDatum != nullptr && onDestroyDatum->Size() > 0)
		{
			ActionList* onDestroyList = onDestroyDatum->Get<Scope>().AssertiveAs<ActionList>();
			worldState.action = onDestroyList;
			onDestroyList->OnDestroy(worldState);
		}
	}

	void Sector::EntitiesOnDestroy(WorldState& worldState)
	{
		DeletePendingDestroyEntities(worldState);
		std::uint32_t i;
		Datum& entities = Entities();
		for (i = 0; i < entities.Size(); i++)
		{
			Entity* entity = entities.Get<Scope>(i).AssertiveAs<Entity>();
			worldState.entity = entity;
			entity->OnDestroy(worldState);
		}

		worldState.entity = nullptr;
	}

	void Sector::ActionsOnDestroy(WorldState& worldState)
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

	void Sector::ReactionsOnDestroy(WorldState& worldState)
	{
		std::uint32_t i;
		Datum* reactions = Find(World::ATTRIBUTE_REACTIONS);
		if (reactions != nullptr)
		{
			for (i = 0; i < reactions->Size(); i++)
			{
				Action* reaction = reactions->Get<Scope>(i).AssertiveAs<Action>();
				worldState.action = reaction;
				reaction->OnDestroy(worldState);
			}
		}
	}

	void Sector::UpdateSectorActions(WorldState& worldState)
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

	void Sector::DeletePendingDestroyEntities(WorldState& worldState)
	{
		std::uint32_t i;
		Datum& entities = Entities();
		for (i = 0; i < entities.Size(); i++)
		{
			Entity* entity = entities.Get<Scope>(i).AssertiveAs<Entity>();
			if (entity->IsPendingDestroy())
			{
				mEntityListByType[entity->TypeIdInstance()].Remove(entity);
				RemoveEntityFromTypeMap(*entity, RTTI::ClassHeirarchy()[entity->TypeIdInstance()]);
				entity->OnDestroy(worldState);
				delete entity;
				--i;		// all elements shifted by 1, if we dont do this, the very next element is skipped
			}
		}
	}

	void Sector::UpdateSectorEntities(WorldState& worldState)
	{
		std::uint32_t i;
		Datum& entities = Entities();
		// size is cached so that if an ActionCreateEntity is encountered, the new Entities Update method is not called in this frame
		// similarly, ActionDestroyEntity will not destroy the entity immediately, it will do it on the next frame update
		std::uint32_t size = entities.Size();
		for (i = 0; i < size; i++)
		{
			Entity* entity = entities.Get<Scope>(i).AssertiveAs<Entity>();
			worldState.entity = entity;
			entity->Update(worldState);
		}
	}

	void Sector::AddEntityToTypeMap(Entity& entity, const std::uint64_t* parentTypeIdPtr)
	{
		if (*parentTypeIdPtr == Attributed::TypeIdClass())
			return;
		mEntityListByType[*parentTypeIdPtr].PushBack(&entity);
		parentTypeIdPtr = RTTI::ClassHeirarchy()[*parentTypeIdPtr];
		AddEntityToTypeMap(entity, parentTypeIdPtr);
	}

	void Sector::RemoveEntityFromTypeMap(Entity& entity, const std::uint64_t* parentTypeIdPtr)
	{
		if (*parentTypeIdPtr == Attributed::TypeIdClass())
			return;
		mEntityListByType[*parentTypeIdPtr].Remove(&entity);
		parentTypeIdPtr = RTTI::ClassHeirarchy()[*parentTypeIdPtr];
		RemoveEntityFromTypeMap(entity, parentTypeIdPtr);
	}

}

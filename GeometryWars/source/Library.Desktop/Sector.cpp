#include "pch.h"
#include "Sector.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(Sector);

	const std::string Sector::ATTRIBUTE_ENTITIES = "entities";
	const std::string Sector::ATTRIBUTE_NAME = "name";

	Sector::Sector() :
		mName(), mEntityListByType()
	{
		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
		AddNestedScope(ATTRIBUTE_ENTITIES);
		AddNestedScope(Entity::ATTRIBUTE_ACTIONS);
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

		//for (auto& pair : mEntityListByType)
		//{
		//	if (entity->Is(pair.first))
		//		pair.second.PushBack(entity);
		//}
		if(!mEntityListByType.ContainsKey(entity->TypeIdInstance()))
			mEntityListByType[entity->TypeIdInstance()].PushBack(entity);

		return *entity;
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
		Datum* beginPlayDatum = Find(World::ATTRIBUTE_BEGIN_PLAY);
		if (beginPlayDatum != nullptr && beginPlayDatum->Size() > 0)
		{
			ActionList* beginPlayList = beginPlayDatum->Get<Scope>().AssertiveAs<ActionList>();
			beginPlayList->Update(worldState);

			std::uint32_t i;
			Datum& entities = Entities();
			for (i = 0; i < entities.Size(); i++)
			{
				Entity* entity = entities.Get<Scope>(i).AssertiveAs<Entity>();
				worldState.entity = entity;
				entity->BeginPlay(worldState);
			}
		}
	}

	void Sector::Update(WorldState& worldState)
	{
		UpdateSectorActions(worldState);
		DeletePendingDestroyEntities();
		UpdateSectorEntities(worldState);
	}

	const Vector<Entity*>& Sector::GetAllEntitiesOfType(std::uint64_t typeId) const
	{
		return mEntityListByType[typeId];
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

	void Sector::DeletePendingDestroyEntities()
	{
		std::uint32_t i;
		Datum& entities = Entities();
		for (i = 0; i < entities.Size(); i++)
		{
			Entity* entity = entities.Get<Scope>(i).AssertiveAs<Entity>();
			if (entity->IsPendingDestroy())
			{
				mEntityListByType[entity->TypeIdInstance()].Remove(entity);
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

	void Sector::AddEntityToTypeMap(RTTI* entity, std::uint64_t typeId)
	{
		if (typeId == Attributed::TypeIdClass())
			return;

		mEntityListByType[typeId].PushBack(entity->AssertiveAs<Entity>());
		//AddEntityToTypeMap(entity, entot)
	}

}

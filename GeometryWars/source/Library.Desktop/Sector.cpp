#include "pch.h"
#include "Sector.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(Sector);

	const std::string Sector::ATTRIBUTE_ENTITIES = "entities";
	const std::string Sector::ATTRIBUTE_NAME = "name";

	Sector::Sector() :
		mName()
	{
		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
		AddNestedScope(ATTRIBUTE_ENTITIES);
		AddNestedScope(Entity::ATTRIBUTE_ACTIONS);
		//AddNestedScope(World::ATTRIBUTE_BEGIN_PLAY);
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

	Entity* Sector::FindEntity(const std::string& entityName) const
	{
		const Datum& entities = Entities();
		for (std::uint32_t i = 0; i < entities.Size(); i++)
		{
			Entity* entity = entities.Get<Scope>(i).As<Entity>();
			assert(entity != nullptr);
			if (entity->Name() == entityName)
				return entity;
		}
		return nullptr;
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
		Datum& actions = Actions();
		std::uint32_t i;
		for (i = 0; i < actions.Size(); i++)
		{
			Action* action = actions.Get<Scope>(i).AssertiveAs<Action>();
			worldState.action = action;
			action->Update(worldState);
		}

		Datum& entities = Entities();
		for (i = 0; i < entities.Size(); i++)
		{
			Entity* entity = entities.Get<Scope>(i).AssertiveAs<Entity>();
			worldState.entity = entity;
			entity->Update(worldState);
		}
	}

}

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
		Populate();
	}

	Sector::Sector(const Sector& rhs) :
		mName(rhs.mName), Attributed(rhs)
	{
		(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
	}

	Sector::Sector(Sector&& rhs) :
		mName(std::move(rhs.mName)), Attributed(std::move(rhs))
	{
		(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
	}

	Sector::~Sector()
	{}

	Sector& Sector::operator=(const Sector& rhs)
	{
		if (this != &rhs)
		{
			mName = rhs.mName;
			Attributed::operator=(rhs);

			(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
		}
		return *this;
	}

	Sector& Sector::operator=(Sector&& rhs)
	{
		if (this != &rhs)
		{
			mName = std::move(rhs.mName);
			Attributed::operator=(std::move(rhs));

			(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
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

	Entity* Sector::FindEntity(const std::string& entityName) const
	{
		const Datum& entities = Entities();
		for (std::uint32_t i = 0; i < entities.Size(); i++)
		{
			Entity* entity = entities.Get<Scope*>(i)->As<Entity>();
			assert(entity != nullptr);
			if (entity->Name() == entityName)
				return entity;
		}
		return nullptr;
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

	void Sector::Update(WorldState& worldState)
	{
		Datum& entities = Entities();
		std::uint32_t i;
		for (i = 0; i < entities.Size(); i++)
		{
			Entity* entity = entities.Get<Scope*>(i)->As<Entity>();
			assert(entity != nullptr);
			worldState.entity = entity;
			entity->Update(worldState);
		}
	}

	Scope* Sector::Clone(const Scope& rhs) const
	{
		if (!rhs.Is(Sector::TypeIdClass()))
			throw std::exception("Given Scope reference is not a Sector.");
		return new Sector(*(rhs.As<Sector>()));
	}

	void Sector::DefinePrescribedAttributes()
	{
		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
		AddNestedScope(ATTRIBUTE_ENTITIES, Scope(), 0);
	}
}

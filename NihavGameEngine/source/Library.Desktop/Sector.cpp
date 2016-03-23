#include "pch.h"
#include "Sector.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(Sector);

	Sector::Sector() :
		mName()
	{
		Populate();
	}

	Sector::Sector(const Sector& rhs) :
		mName(rhs.mName)
	{
		Populate();
	}

	Sector::Sector(Sector&& rhs) :
		mName(std::move(rhs.mName)), Attributed(std::move(rhs))
	{}

	Sector::~Sector()
	{}

	Sector& Sector::operator=(const Sector& rhs)
	{
		if (this != &rhs)
		{
			mName = rhs.mName;
			Attributed::operator=(rhs);

			(*this)[ATTRIBUTE_NAME_NAME].SetStorage(&mName, 1);
		}
		return *this;
	}

	Sector& Sector::operator=(Sector&& rhs)
	{
		if (this != &rhs)
		{
			mName = std::move(rhs.mName);
			Attributed::operator=(std::move(rhs));

			(*this)[ATTRIBUTE_NAME_NAME].SetStorage(&mName, 1);
		}

		return *this;
	}
	
	std::string Sector::Name() const
	{
		return mName;
	}

	void Sector::SetName(const std::string& name)
	{
		mName = name;
	}

	Datum& Sector::Entities()
	{
		return Append(ATTRIBUTE_NAME_ENTITY);
	}

	Entity& Sector::CreateEntity(const std::string& entityClassName, const std::string& entityInstanceName)
	{
		Datum& entities = Entities();
		
		EntityFactory entityFactory;
		if (Factory<RTTI>::Find(entityClassName) == nullptr)
		{
			std::stringstream str;
			str << "Class name " << entityClassName << " not found.";
			throw std::exception(str.str().c_str());
		}
		Entity* entity = Factory<RTTI>::Create(entityClassName)->As<Entity>();

		entity->SetName(entityInstanceName);
		entity->SetSector(*this);

		entities.Set(*entity, entities.Size());

		return *entity;
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
		parent.Adopt(ATTRIBUTE_NAME_NAME, *this);
	}

	void Sector::Update(WorldState& worldState)
	{
		Datum& entities = Entities();
		std::uint32_t i;
		for (i = 0; i < entities.Size(); i++)
		{
			Entity* entity = entities.Get<Scope*>(i)->As<Entity>();
			worldState.entity = entity;
			entity->Update(worldState);
		}
	}

	void Sector::DefinePrescribedAttributes()
	{
		AddExternalAttribute(ATTRIBUTE_NAME_NAME, 1, &mName);
	}
}

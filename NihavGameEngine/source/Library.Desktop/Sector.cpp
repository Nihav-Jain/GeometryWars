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

	Entity& Sector::CreateEntity(const std::string& entityClassName, const std::string& entityInstanceName)
	{
		Entity* entity = Factory<Entity>::Create(entityClassName);
		//if(entity == nullptr)
		//{
		//	std::stringstream str;
		//	str << "Class name " << entityClassName << " not found.";
		//	throw std::exception(str.str().c_str());
		//}
		assert(entity != nullptr);

		entity->SetName(entityInstanceName);
		entity->SetSector(*this);

		Adopt(ATTRIBUTE_ENTITIES, *entity);

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
		parent.Adopt(ATTRIBUTE_NAME, *this);
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

	void Sector::DefinePrescribedAttributes()
	{
		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
		AddNestedScope(ATTRIBUTE_ENTITIES, Scope(), 0);
	}
}

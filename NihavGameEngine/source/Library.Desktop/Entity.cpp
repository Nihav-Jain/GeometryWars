#include "pch.h"
#include "Entity.h"
#include "Sector.h"

namespace Library
{
	RTTI_DEFINITIONS(Entity);

	const std::uint32_t Entity::NUM_RESERVED_PRESCRIBED_ATTRIBUTES = 3;
	const std::string Entity::ATTRIBUTE_NAME = "name";

	Entity::Entity() :
		mName()
	{
		Populate();
	}

	Entity::Entity(const Entity& rhs) :
		mName(rhs.mName), Attributed(rhs)
	{
		(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
	}

	Entity::Entity(Entity&& rhs) :
		mName(std::move(rhs.mName)), Attributed(std::move(rhs))
	{
		(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
	}


	Entity::~Entity()
	{
	}

	Entity& Entity::operator=(const Entity& rhs)
	{
		if (this != &rhs)
		{
			mName = rhs.mName;
			Attributed::operator=(rhs);

			(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
		}
		return *this;
	}

	Entity& Entity::operator=(Entity&& rhs)
	{
		if (this != &rhs)
		{
			mName = std::move(rhs.mName);
			Attributed::operator=(std::move(rhs));

			(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
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

	void Entity::Update(WorldState& worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
	}

	Scope* Entity::Clone(const Scope& rhs) const
	{
		if (!rhs.Is(Entity::TypeIdClass()))
			throw std::exception("Given Scope reference is not an Entity.");
		return new Entity(*(rhs.As<Entity>()));
	}

	void Entity::DefinePrescribedAttributes()
	{
		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
	}
}

#include "pch.h"
#include "Entity.h"
#include "Sector.h"

namespace Library
{
	RTTI_DEFINITIONS(Entity);

	Entity::Entity() :
		mName()
	{
		Populate();
	}

	Entity::Entity(const Entity& rhs) :
		mName(rhs.mName)
	{
		Populate();
	}

	Entity::Entity(Entity&& rhs) :
		mName(std::move(rhs.mName)), Attributed(std::move(rhs))
	{}


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
		parent.Adopt(mName, *this);
	}

	void Entity::Update(WorldState& worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
	}

	void Entity::DefinePrescribedAttributes()
	{
		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
	}
}

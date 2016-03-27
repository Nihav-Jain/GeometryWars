#include "pch.h"
#include "Entity.h"
#include "Sector.h"
#include "Action.h"

namespace Library
{
	RTTI_DEFINITIONS(Entity);

	const std::uint32_t Entity::NUM_RESERVED_PRESCRIBED_ATTRIBUTES = 3;
	const std::string Entity::ATTRIBUTE_NAME = "name";
	const std::string Entity::ATTRIBUTE_ACTIONS = "actions";
	
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

	Datum& Entity::Actions()
	{
		return operator[](ATTRIBUTE_ACTIONS);
	}

	const Datum& Entity::Actions() const
	{
		return *Find(ATTRIBUTE_ACTIONS);
	}

	Action& Entity::CreateAction(const std::string& actionClassName, const std::string& actionInstanceName)
	{
		Action* action = Factory<Action>::Create(actionClassName);
		assert(action != nullptr);
		action->SetName(actionInstanceName);
		
		Adopt(ATTRIBUTE_ACTIONS, *action);

		return *action;
	}

	Action* Entity::FindAction(const std::string& actionName) const
	{
		const Datum& actions = Actions();
		for (std::uint32_t i = 0; i < actions.Size(); i++)
		{
			Action* action = actions.Get<Scope*>(i)->As<Action>();
			assert(action != nullptr);
			if (action->Name() == actionName)
				return action;
		}
		return nullptr;
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
		Datum& actions = Actions();
		std::uint32_t i;
		for (i = 0; i < actions.Size(); i++)
		{
			Action* action = actions.Get<Scope*>(i)->As<Action>();
			assert(action != nullptr);
			worldState.action = action;
			action->Update(worldState);
		}
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
		AddNestedScope(ATTRIBUTE_ACTIONS, Scope(), 0);
	}
}

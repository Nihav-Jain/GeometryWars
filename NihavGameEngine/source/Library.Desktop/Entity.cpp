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

	void Entity::DefinePrescribedAttributes()
	{
		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
		AddNestedScope(ATTRIBUTE_ACTIONS, Scope(), 0);
	}
}

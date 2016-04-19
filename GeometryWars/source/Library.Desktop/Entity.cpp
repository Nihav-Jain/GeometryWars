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
		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
		AddNestedScope(ATTRIBUTE_ACTIONS);
		//AddNestedScope(World::ATTRIBUTE_BEGIN_PLAY);
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

	Action* Entity::FindAction(const std::string& actionName) const
	{
		return Action::FindAction(actionName, Actions());
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

	void Entity::BeginPlay(WorldState& worldState)
	{
		Datum* beginPlayDatum = Find(World::ATTRIBUTE_BEGIN_PLAY);
		if (beginPlayDatum != nullptr && beginPlayDatum->Size() > 0)
		{
			ActionList* beginPlayList = beginPlayDatum->Get<Scope>().AssertiveAs<ActionList>();
			beginPlayList->Update(worldState);
		}
	}

	void Entity::Update(WorldState& worldState)
	{
		Datum& actions = Actions();
		std::uint32_t i;
		for (i = 0; i < actions.Size(); i++)
		{
			Action* action = actions.Get<Scope>(i).AssertiveAs<Action>();
			worldState.action = action;
			action->Update(worldState);
		}
	}

}

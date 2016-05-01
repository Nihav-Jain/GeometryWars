#include "pch.h"
#include "Action.h"

namespace Library
{
	RTTI_DEFINITIONS(Action, Attributed);

	const std::uint32_t Action::NUM_RESERVED_PRESCRIBED_ATTRIBUTES = 7;
	const std::string Action::ATTRIBUTE_NAME = "name";
	const std::string Action::ATTRIBUTE_CAN_EVER_TICK = "canEverTick";
	const std::string Action::ATTRIBUTE_OWNER_ENTITY = "ownerEntity";
	const std::string Action::ATTRIBUTE_OWNER_ACTION = "ownerAction";

	Action::Action() :
		mName(), mWorld(new Datum()), mSector(new Datum()), mEntity(new Datum()), mAction(new Datum())
	{
		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
		AddInternalAttribute(ATTRIBUTE_CAN_EVER_TICK, true, 1);

		AddExternalAttribute(Sector::ATTRIBUTE_OWNER_WORLD, 1, &mWorld);
		AddExternalAttribute(Entity::ATTRIBUTE_OWNER_SECTOR, 1, &mSector);
		AddExternalAttribute(ATTRIBUTE_OWNER_ENTITY, 1, &mEntity);
		AddExternalAttribute(ATTRIBUTE_OWNER_ACTION, 1, &mAction);
	}

	Action::~Action()
	{
		delete mWorld;
		delete mSector;
		delete mEntity;
		delete mAction;
	}

	const std::string& Action::Name() const
	{
		return mName;
	}

	void Action::SetName(const std::string& name)
	{
		mName = name;
	}

	void Action::BeginPlay(WorldState& worldState)
	{
		*mWorld = *worldState.world;
		if (worldState.sector != nullptr)
			*mSector = *worldState.sector;
		if (worldState.entity != nullptr)
			*mEntity = *worldState.entity;
		if (worldState.action != nullptr)
			*mAction = *worldState.action;
	}

	void Action::OnDestroy(WorldState& worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
	}

	void Action::PostParsingProcess()
	{}

	Action* Action::FindAction(const std::string& actionName, const Datum& actions)
	{
		for (std::uint32_t i = 0; i < actions.Size(); i++)
		{
			Action* action = actions.Get<Scope>(i).As<Action>();
			assert(action != nullptr);
			if (action->Name() == actionName)
				return action;
		}
		return nullptr;
	}

	Action& Action::CreateAction(const std::string& actionClassName, const std::string& actionInstanceName, Scope& parentScope, const std::string& attributeName)
	{
		Action* action = Factory<Action>::Create(actionClassName);
		assert(action != nullptr);
		action->SetName(actionInstanceName);

		parentScope.Adopt(attributeName, *action);

		return *action;
	}

}

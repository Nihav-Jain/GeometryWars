#include "pch.h"
#include "ActionCreateEntityFromFile.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionCreateEntityFromFile, Action);

	const std::string ActionCreateEntityFromFile::ATTRIBUTE_ENTITY_CLASS_NAME = "className";
	const std::string ActionCreateEntityFromFile::ATTRIBUTE_ENTITY_INSTANCE_NAME = "instanceName";

	ActionCreateEntityFromFile::ActionCreateEntityFromFile()
	{
		AddInternalAttribute(ATTRIBUTE_ENTITY_CLASS_NAME, "", 0);
		AddInternalAttribute(ATTRIBUTE_ENTITY_INSTANCE_NAME, "", 0);
	}

	ActionCreateEntityFromFile::ActionCreateEntityFromFile(const ActionCreateEntityFromFile& rhs) :
		Action::Action(rhs)
	{}

	ActionCreateEntityFromFile::ActionCreateEntityFromFile(ActionCreateEntityFromFile&& rhs) :
		Action::Action(std::move(rhs))
	{}

	ActionCreateEntityFromFile& ActionCreateEntityFromFile::operator=(const ActionCreateEntityFromFile& rhs)
	{
		Action::operator=(rhs);
		return *this;
	}

	ActionCreateEntityFromFile& ActionCreateEntityFromFile::operator=(ActionCreateEntityFromFile&& rhs)
	{
		Action::operator=(std::move(rhs));
		return *this;
	}


	void ActionCreateEntityFromFile::Update(WorldState& worldState)
	{
		assert(worldState.sector != nullptr);

		Entity* entityCopy = worldState.world->ClassDefinitionContainer.FindEntity((*this)[ATTRIBUTE_ENTITY_CLASS_NAME].Get<std::string>());
		assert(entityCopy != nullptr);

		Entity* newEntity = entityCopy->Clone(*entityCopy)->AssertiveAs<Entity>();

		worldState.sector->AdoptEntity(*newEntity, (*this)[ATTRIBUTE_ENTITY_INSTANCE_NAME].Get<std::string>());
		newEntity->BeginPlay(worldState);
	}

	Scope* ActionCreateEntityFromFile::Clone(const Scope& rhs) const
	{
		ActionCreateEntityFromFile& action = *rhs.AssertiveAs<ActionCreateEntityFromFile>();
		return new ActionCreateEntityFromFile(action);
	}

}

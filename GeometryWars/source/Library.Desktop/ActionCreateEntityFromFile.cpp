#include "pch.h"
#include "ActionCreateEntityFromFile.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionCreateEntityFromFile, Action);

	const std::string ActionCreateEntityFromFile::ATTRIBUTE_FILE_PATH = "filePath";
	const std::string ActionCreateEntityFromFile::ATTRIBUTE_ENTITY_CLASS_NAME = "className";
	const std::string ActionCreateEntityFromFile::ATTRIBUTE_ENTITY_INSTANCE_NAME = "instanceName";
	const std::string ActionCreateEntityFromFile::ATTRIBUTE_NEW_ENTITY = "newEntity";

	ActionCreateEntityFromFile::ActionCreateEntityFromFile() :
		mNewEntity(new Datum())
	{
		AddInternalAttribute(ATTRIBUTE_ENTITY_CLASS_NAME, "", 0);
		AddInternalAttribute(ATTRIBUTE_ENTITY_INSTANCE_NAME, "", 0);
		AddInternalAttribute(ATTRIBUTE_NEW_ENTITY, mNewEntity, 1);
	}

	ActionCreateEntityFromFile::~ActionCreateEntityFromFile()
	{
		delete mNewEntity;
	}

	ActionCreateEntityFromFile::ActionCreateEntityFromFile(const ActionCreateEntityFromFile& rhs) :
		Action::Action(rhs), mNewEntity(new Datum(*rhs.mNewEntity))
	{
		(*this)[ATTRIBUTE_NEW_ENTITY] = mNewEntity;
	}

	ActionCreateEntityFromFile::ActionCreateEntityFromFile(ActionCreateEntityFromFile&& rhs) :
		Action::Action(std::move(rhs)), mNewEntity(rhs.mNewEntity)
	{
		(*this)[ATTRIBUTE_NEW_ENTITY] = mNewEntity;
		rhs.mNewEntity = nullptr;
	}

	ActionCreateEntityFromFile& ActionCreateEntityFromFile::operator=(const ActionCreateEntityFromFile& rhs)
	{
		if (this != &rhs)
		{
			mNewEntity = new Datum(*rhs.mNewEntity);
			Action::operator=(rhs);
			(*this)[ATTRIBUTE_NEW_ENTITY] = mNewEntity;
		}
		return *this;
	}

	ActionCreateEntityFromFile& ActionCreateEntityFromFile::operator=(ActionCreateEntityFromFile&& rhs)
	{
		if (this != &rhs)
		{
			mNewEntity = rhs.mNewEntity;
			Action::operator=(std::move(rhs));
			(*this)[ATTRIBUTE_NEW_ENTITY] = mNewEntity;
			rhs.mNewEntity = nullptr;
		}
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
		*mNewEntity = *newEntity;
	}

	Scope* ActionCreateEntityFromFile::Clone(const Scope& rhs) const
	{
		ActionCreateEntityFromFile& action = *rhs.AssertiveAs<ActionCreateEntityFromFile>();
		return new ActionCreateEntityFromFile(action);
	}

}

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
		*mNewEntity = *newEntity;
		

		//Uncomment this section if the build is breaking
		/*
		assert(worldState.sector != nullptr);

		SharedDataTable* sharedDataPtr = worldState.world->ParseMaster().GetSharedData()->AssertiveAs<SharedDataTable>();
		Scope* cacheCurrentScope = sharedDataPtr->CurrentScopePtr;

		sharedDataPtr->CurrentScopePtr = worldState.sector;

		sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::WORLD_START);
		sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::SECTOR_START);
		sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);

		bool parsingSuccessful = worldState.world->ParseMaster().ParseFromFile((*this)[ATTRIBUTE_FILE_PATH].Get<std::string>());

		sharedDataPtr->CurrentScopePtr = cacheCurrentScope;
		sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::SECTOR_END);
		sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::WORLD_END);

		if (!parsingSuccessful)
		{
			std::stringstream str;
			str << "Error in parsing file - " << (*this)[ATTRIBUTE_FILE_PATH].Get<std::string>();
			throw std::exception(str.str().c_str());
		}

		Datum& sectorEntities = worldState.sector->Entities();
		Entity& newEntity = *sectorEntities.Get<Scope>(sectorEntities.Size() - 1).AssertiveAs<Entity>();
		newEntity.BeginPlay(worldState);
		*mNewEntity = newEntity;
		*/
	}

	Scope* ActionCreateEntityFromFile::Clone(const Scope& rhs) const
	{
		ActionCreateEntityFromFile& action = *rhs.AssertiveAs<ActionCreateEntityFromFile>();
		return new ActionCreateEntityFromFile(action);
	}

}

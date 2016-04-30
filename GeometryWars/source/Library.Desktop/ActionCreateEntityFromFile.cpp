#include "pch.h"
#include "ActionCreateEntityFromFile.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionCreateEntityFromFile, Action);

	const std::string ActionCreateEntityFromFile::ATTRIBUTE_FILE_PATH = "filePath";

	ActionCreateEntityFromFile::ActionCreateEntityFromFile()
	{
		AddInternalAttribute(ATTRIBUTE_FILE_PATH, "", 0);
	}

	void ActionCreateEntityFromFile::Update(WorldState& worldState)
	{
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
	}

}

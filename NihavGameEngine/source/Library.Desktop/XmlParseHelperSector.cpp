#include "pch.h"
#include "XmlParseHelperSector.h"

namespace Library
{
	bool XmlParseHelperSector::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;

		if (elementName != ELEMENT_NAME)
			return false;

		if (!attributes.ContainsKey(ATTRIBUTE_NAME))
			throw std::exception("Invalid syntax for <scope>. Missing attribute: name");

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::SECTOR_START))
			throw std::exception("Invalid script syntax");
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		assert(transitionToStateRouter);

		assert(sharedDataPtr->CurrentScopePtr->Is(World::TypeIdClass()));
		World* world = static_cast<World*>(sharedDataPtr->CurrentScopePtr);

		sharedDataPtr->CurrentScopePtr = &(world->CreateSector(attributes[ATTRIBUTE_NAME]));
		return true;
	}

	bool XmlParseHelperSector::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (elementName != ELEMENT_NAME)
			return false;

		bool transitionToSectorEnd = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::SECTOR_END);
		assert(transitionToSectorEnd);
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		assert(transitionToStateRouter);

		sharedDataPtr->CurrentScopePtr = sharedDataPtr->CurrentScopePtr->GetParent();

		return true;
	}

	IXmlParseHelper* XmlParseHelperSector::Clone() const
	{
		return new XmlParseHelperSector();
	}
}

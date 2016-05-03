#include "pch.h"
#include "XmlParseHelperDefineSector.h"

namespace Library
{
	bool XmlParseHelperDefineSector::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
	{
		UNREFERENCED_PARAMETER(attributes);

		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;

		if (elementName != ELEMENT_NAME)
			return false;

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::WORLD_START))
			throw std::exception("Invalid script syntax");
		sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);

		mOriginalParent = sharedDataPtr->CurrentScopePtr;
		World& currenWorld = *sharedDataPtr->CurrentScopePtr->AssertiveAs<World>();
		if (currenWorld.SectorDefinitionContainer == nullptr)
			currenWorld.SectorDefinitionContainer = new World(*currenWorld.GetWorldState().mGameTime, currenWorld.ParseMaster());
		sharedDataPtr->CurrentScopePtr = currenWorld.SectorDefinitionContainer;

		return true;
	}

	bool XmlParseHelperDefineSector::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (elementName != ELEMENT_NAME)
			return false;

		sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::WORLD_END);

		sharedDataPtr->CurrentScopePtr = mOriginalParent;

		return true;
	}

	IXmlParseHelper* XmlParseHelperDefineSector::Clone() const
	{
		return new XmlParseHelperDefineSector();
	}
}

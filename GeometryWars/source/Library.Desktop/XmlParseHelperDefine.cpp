#include "pch.h"
#include "XmlParseHelperDefine.h"

namespace Library
{
	bool XmlParseHelperDefine::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
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
		sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::SECTOR_START);
		sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);

		mOriginalParent = sharedDataPtr->CurrentScopePtr;
		sharedDataPtr->CurrentScopePtr = &sharedDataPtr->CurrentScopePtr->AssertiveAs<World>()->ClassDefinitionContainer;

		return true;
	}

	bool XmlParseHelperDefine::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (elementName != ELEMENT_NAME)
			return false;

		bool transitionToEntityEnd = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::SECTOR_END);
		UNREFERENCED_PARAMETER(transitionToEntityEnd);
		assert(transitionToEntityEnd);
		sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::WORLD_END);

		sharedDataPtr->CurrentScopePtr = mOriginalParent;

		return true;
	}

	IXmlParseHelper* XmlParseHelperDefine::Clone() const
	{
		return new XmlParseHelperDefine();
	}
}

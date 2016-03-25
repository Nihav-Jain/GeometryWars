#include "pch.h"
#include "XmlParseHelperTable.h"

namespace Library
{

	bool XmlParseHelperTable::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;

		if (elementName != ELEMENT_SCOPE)
			return false;

		if (!attributes.ContainsKey(ATTRIBUTE_NAME))
			throw std::exception("Invalid syntax for <scope>. Missing attribute: name");

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::SCOPE_START))
			throw std::exception("Invalid script syntax");
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		assert(transitionToStateRouter);

		sharedDataPtr->CurrentScopePtr =  &(sharedDataPtr->CurrentScopePtr->AppendScope(attributes[ATTRIBUTE_NAME]));

		return true;
	}

	bool XmlParseHelperTable::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (elementName != ELEMENT_SCOPE)
			return false;

		bool transitionToScopeEnd = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::SCOPE_END);
		assert(transitionToScopeEnd);
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		assert(transitionToStateRouter);

		sharedDataPtr->CurrentScopePtr = sharedDataPtr->CurrentScopePtr->GetParent();
		return true;
	}

	IXmlParseHelper* XmlParseHelperTable::Clone() const
	{
		return new XmlParseHelperTable();
	}

}

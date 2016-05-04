#include "pch.h"
#include "XmlParseHelperWorld.h"
#include "Game.h"

namespace Library
{
	bool XmlParseHelperWorld::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;

		if (elementName != ELEMENT_NAME)
			return false;

		if (!attributes.ContainsKey(ATTRIBUTE_NAME))
			throw std::exception("ClearScreen syntax for <world>. Missing attribute: name");

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::WORLD_START))
			throw std::exception("ClearScreen script syntax");
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		UNREFERENCED_PARAMETER(transitionToStateRouter);
		assert(transitionToStateRouter);

		sharedDataPtr->CurrentScopePtr->AssertiveAs<World>()->SetName(attributes[ATTRIBUTE_NAME]);

		return true;
	}

	bool XmlParseHelperWorld::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (elementName != ELEMENT_NAME)
			return false;

		bool transitionToWorldEnd = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::WORLD_END);
		UNREFERENCED_PARAMETER(transitionToWorldEnd);
		assert(transitionToWorldEnd);

		sharedDataPtr->CurrentScopePtr = sharedDataPtr->CurrentScopePtr->GetParent();

		return true;
	}

	IXmlParseHelper* XmlParseHelperWorld::Clone() const
	{
		return new XmlParseHelperWorld();
	}
}

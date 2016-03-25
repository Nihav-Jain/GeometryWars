#include "pch.h"
#include "XmlParseHelperWorld.h"

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
			throw std::exception("Invalid syntax for <scope>. Missing attribute: name");

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::WORLD_START))
			throw std::exception("Invalid script syntax");
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		assert(transitionToStateRouter);

		World* world = new World();
		sharedDataPtr->CurrentScopePtr->Adopt(attributes[ATTRIBUTE_NAME], *world);
		sharedDataPtr->CurrentScopePtr = world;

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
		assert(transitionToWorldEnd);

		sharedDataPtr->CurrentScopePtr = sharedDataPtr->CurrentScopePtr->GetParent();

		return true;
	}

	IXmlParseHelper* XmlParseHelperWorld::Clone() const
	{
		return new XmlParseHelperWorld();
	}
}

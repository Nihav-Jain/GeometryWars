#include "pch.h"
#include "XmlParseHelperEntity.h"

namespace Library
{
	bool XmlParseHelperEntity::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;

		if (elementName != ELEMENT_NAME)
			return false;

		if (!attributes.ContainsKey(ATTRIBUTE_NAME))
			throw std::exception("Invalid syntax for <entity>. Missing attribute: name");
		if (!attributes.ContainsKey(ATTRIBUTE_CLASS))
			throw std::exception("Invalid syntax for <entity>. Missing attribute: class");

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::ENTITY_START))
			throw std::exception("Invalid script syntax");
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		assert(transitionToStateRouter);

		assert(sharedDataPtr->CurrentScopePtr->Is(Sector::TypeIdClass()));
		Sector* sector = static_cast<Sector*>(sharedDataPtr->CurrentScopePtr);

		sharedDataPtr->CurrentScopePtr = &(sector->CreateEntity(attributes[ATTRIBUTE_CLASS], attributes[ATTRIBUTE_NAME]));
		return true;
	}

	bool XmlParseHelperEntity::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (elementName != ELEMENT_NAME)
			return false;

		bool transitionToEntityEnd = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::ENTITY_END);
		assert(transitionToEntityEnd);
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		assert(transitionToStateRouter);

		// test for entity prescribed attributes.

		sharedDataPtr->CurrentScopePtr = sharedDataPtr->CurrentScopePtr->GetParent();

		return true;
	}

	IXmlParseHelper* XmlParseHelperEntity::Clone() const
	{
		return new XmlParseHelperEntity();
	}
}

#include "pch.h"
#include "XmlParseHelperGameObject.h"

namespace Library
{
	bool XmlParseHelperGameObject::StartElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & elementName, const Hashmap<std::string, std::string>& attributes)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;

		if (elementName != ELEMENT_NAME)
			return false;

		if (!attributes.ContainsKey(ATTRIBUTE_NAME))
			throw std::exception("ClearScreen syntax for <gameobject>. Missing attribute: name");
		if (!attributes.ContainsKey(ATTRIBUTE_CLASS))
			throw std::exception("ClearScreen syntax for <gameobject>. Missing attribute: class");


		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::ENTITY_START))
			throw std::exception("Invalid script syntax");
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		UNREFERENCED_PARAMETER(transitionToStateRouter);
		assert(transitionToStateRouter);

		assert(sharedDataPtr->CurrentScopePtr->Is(Sector::TypeIdClass()));
		Sector* sector = static_cast<Sector*>(sharedDataPtr->CurrentScopePtr);

		GameObject* gameObject = sector->CreateEntity(attributes[ATTRIBUTE_CLASS], attributes[ATTRIBUTE_NAME]).As<GameObject>();

		sharedDataPtr->CurrentScopePtr = gameObject;
		return true;
	}

	bool XmlParseHelperGameObject::EndElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & elementName)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (elementName != ELEMENT_NAME)
			return false;

		bool transitionToGameObjectEnd = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::ENTITY_END);
		UNREFERENCED_PARAMETER(transitionToGameObjectEnd);
		assert(transitionToGameObjectEnd);
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		UNREFERENCED_PARAMETER(transitionToStateRouter);
		assert(transitionToStateRouter);

		GameObject* currentGameObject = sharedDataPtr->CurrentScopePtr->As<GameObject>();
		UNREFERENCED_PARAMETER(currentGameObject);
		assert(currentGameObject != nullptr);
		sharedDataPtr->CurrentScopePtr = sharedDataPtr->CurrentScopePtr->GetParent();

		return true;
	}

	IXmlParseHelper* XmlParseHelperGameObject::Clone() const
	{
		return new XmlParseHelperGameObject();
	}
}

#include "pch.h"
#include "XmlParseHelperBeginPlay.h"
#include "Entity.h"
#include "ActionList.h"
#include "Reaction.h"

namespace Library
{
	const std::string XmlParseHelperBeginPlay::ELEMENT_NAME = World::ATTRIBUTE_BEGIN_PLAY;

	XmlParseHelperBeginPlay::XmlParseHelperBeginPlay()
	{
	}

	bool XmlParseHelperBeginPlay::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
	{
		UNREFERENCED_PARAMETER(attributes);
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;

		if (elementName != ELEMENT_NAME)
			return false;

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::ACTION_START))
			throw std::exception("ClearScreen script syntax");
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		UNREFERENCED_PARAMETER(transitionToStateRouter);
		assert(transitionToStateRouter);
		
		Scope* parent = sharedDataPtr->CurrentScopePtr;
		Action* beginPlayList = &Action::CreateAction("ActionList", World::ATTRIBUTE_BEGIN_PLAY, *parent, World::ATTRIBUTE_BEGIN_PLAY);

		sharedDataPtr->CurrentScopePtr = beginPlayList;

		return true;
	}

	bool XmlParseHelperBeginPlay::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (elementName != ELEMENT_NAME)
			return false;

		bool transitionToActionEnd = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::ACTION_END);
		UNREFERENCED_PARAMETER(transitionToActionEnd);
		assert(transitionToActionEnd);
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		UNREFERENCED_PARAMETER(transitionToStateRouter);
		assert(transitionToStateRouter);

		sharedDataPtr->CurrentScopePtr = sharedDataPtr->CurrentScopePtr->GetParent();

		return true;
	}

	IXmlParseHelper* XmlParseHelperBeginPlay::Clone() const
	{
		return new XmlParseHelperBeginPlay();
	}
}

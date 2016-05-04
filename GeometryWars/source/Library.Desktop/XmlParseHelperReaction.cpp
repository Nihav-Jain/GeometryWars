#include "pch.h"
#include "XmlParseHelperReaction.h"
#include "Entity.h"
#include "ActionList.h"
#include "Reaction.h"

namespace Library
{
	const std::string XmlParseHelperReaction::ELEMENT_NAME = "reaction";
	const std::string XmlParseHelperReaction::ATTRIBUTE_CLASS = "class";
	const std::string XmlParseHelperReaction::ATTRIBUTE_NAME = "name";

	XmlParseHelperReaction::XmlParseHelperReaction()
	{
	}

	bool XmlParseHelperReaction::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;

		if (elementName != ELEMENT_NAME)
			return false;

		if (!attributes.ContainsKey(ATTRIBUTE_CLASS))
		{
			std::stringstream str;
			str << "ClearScreen syntax for <" << ELEMENT_NAME << ">. Missing attribute: " << ATTRIBUTE_CLASS;
			throw std::exception(str.str().c_str());
		}
		if (!attributes.ContainsKey(ATTRIBUTE_NAME))
		{
			std::stringstream str;
			str << "ClearScreen syntax for <" << ELEMENT_NAME << ">. Missing attribute: " << ATTRIBUTE_NAME;
			throw std::exception(str.str().c_str());
		}

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::ACTION_START))
			throw std::exception("ClearScreen script syntax");
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		UNREFERENCED_PARAMETER(transitionToStateRouter);
		assert(transitionToStateRouter);
		
		Attributed* parent = sharedDataPtr->CurrentScopePtr->AssertiveAs<Attributed>();
		Reaction* reaction = Factory<Reaction>::Create(attributes[ATTRIBUTE_CLASS]);
		assert(reaction != nullptr);
		reaction->SetName(attributes[ATTRIBUTE_NAME]);
		parent->Adopt(World::ATTRIBUTE_REACTIONS, *reaction);

		sharedDataPtr->CurrentScopePtr = reaction;

		return true;
	}

	bool XmlParseHelperReaction::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName)
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

	IXmlParseHelper* XmlParseHelperReaction::Clone() const
	{
		return new XmlParseHelperReaction();
	}
}

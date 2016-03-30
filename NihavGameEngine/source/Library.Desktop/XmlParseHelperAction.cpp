#include "pch.h"
#include "XmlParseHelperAction.h"
#include "Entity.h"
#include "ActionList.h"

namespace Library
{
	const std::string XmlParseHelperAction::ATTRIBUTE_CLASS = "class";
	const std::string XmlParseHelperAction::ATTRIBUTE_NAME = "name";

	XmlParseHelperAction::XmlParseHelperAction() :
		mElementName("action"), mRequiredAttributes(), mDerivedActionClassName(), mActionInstanceName()
	{
		mRequiredAttributes.PushBack(ATTRIBUTE_NAME);
		mRequiredAttributes.PushBack(ATTRIBUTE_CLASS);
	}

	bool XmlParseHelperAction::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;

		if (elementName != mElementName)
			return false;

		for (auto& attributeName : mRequiredAttributes)
		{
			if (!attributes.ContainsKey(attributeName))
			{
				std::stringstream str;
				str << "Invalid syntax for <" << mElementName << ">. Missing attribute: " << attributeName;
				throw std::exception(str.str().c_str());
			}
		}

		if (mRequiredAttributes.Find(ATTRIBUTE_CLASS) != mRequiredAttributes.end())
			mDerivedActionClassName = attributes[ATTRIBUTE_CLASS];
		if (mRequiredAttributes.Find(ATTRIBUTE_NAME) != mRequiredAttributes.end())
			mActionInstanceName = attributes[ATTRIBUTE_NAME];

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::ACTION_START))
			throw std::exception("Invalid script syntax");
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		UNREFERENCED_PARAMETER(transitionToStateRouter);
		assert(transitionToStateRouter);

		if (sharedDataPtr->CurrentScopePtr->Is(Entity::TypeIdClass()))
		{
			Entity* entity = static_cast<Entity*>(sharedDataPtr->CurrentScopePtr);
			Action& currentAction = entity->CreateAction(mDerivedActionClassName, mActionInstanceName);
			ParseActionAttributes(currentAction, attributes);
			sharedDataPtr->CurrentScopePtr = &currentAction;
		}
		else if(sharedDataPtr->CurrentScopePtr->Is(ActionList::TypeIdClass()))
		{
			ActionList* actionList = static_cast<ActionList*>(sharedDataPtr->CurrentScopePtr);
			Action& currentAction = actionList->CreateAction(mDerivedActionClassName, mActionInstanceName);
			ParseActionAttributes(currentAction, attributes);
			sharedDataPtr->CurrentScopePtr = &currentAction;
		}
		return true;
	}

	bool XmlParseHelperAction::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (elementName != mElementName)
			return false;

		bool transitionToActionEnd = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::ACTION_END);
		UNREFERENCED_PARAMETER(transitionToActionEnd);
		assert(transitionToActionEnd);
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		UNREFERENCED_PARAMETER(transitionToStateRouter);
		assert(transitionToStateRouter);

		Action* currentAction = sharedDataPtr->CurrentScopePtr->As<Action>();

		 //test for action prescribed attributes.
		std::uint32_t numReservedPrescribedAttributes = Action::NUM_RESERVED_PRESCRIBED_ATTRIBUTES;
		for (std::uint32_t i = numReservedPrescribedAttributes; i < currentAction->AuxiliaryBegin(); i++)
		{
			if (currentAction->operator[](i).Size() < 1 )
			{
				//std::stringstream str;
				//str << "Prescribed Attribute #" << i << " of instance " << currentAction->Name() << " not initialized.";
				//throw std::exception(str.str().c_str());
			}
		}
		
		currentAction->PostParsingProcess();

		sharedDataPtr->CurrentScopePtr = sharedDataPtr->CurrentScopePtr->GetParent();

		return true;
	}

	IXmlParseHelper* XmlParseHelperAction::Clone() const
	{
		return new XmlParseHelperAction();
	}

	void XmlParseHelperAction::ParseActionAttributes(Action& currentAction, const Hashmap<std::string, std::string>& attributes)
	{
		UNREFERENCED_PARAMETER(currentAction);
		UNREFERENCED_PARAMETER(attributes);
	}
}

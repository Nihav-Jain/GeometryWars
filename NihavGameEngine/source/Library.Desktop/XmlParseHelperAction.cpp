#include "pch.h"
#include "XmlParseHelperAction.h"

namespace Library
{

	const std::string XmlParseHelperAction::ELEMENT_NAME	= "action";
	const std::string XmlParseHelperAction::ATTRIBUTE_CLASS = "class";
	const std::string XmlParseHelperAction::ATTRIBUTE_NAME	= "name";

	bool XmlParseHelperAction::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;

		if (elementName != ELEMENT_NAME)
			return false;

		if (!attributes.ContainsKey(ATTRIBUTE_NAME))
			throw std::exception("Invalid syntax for <action>. Missing attribute: name");
		if (!attributes.ContainsKey(ATTRIBUTE_CLASS))
			throw std::exception("Invalid syntax for <action>. Missing attribute: class");

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::ACTION_START))
			throw std::exception("Invalid script syntax");
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		UNREFERENCED_PARAMETER(transitionToStateRouter);
		assert(transitionToStateRouter);

		assert(sharedDataPtr->CurrentScopePtr->Is(Entity::TypeIdClass()));
		Entity* entity = static_cast<Entity*>(sharedDataPtr->CurrentScopePtr);

		sharedDataPtr->CurrentScopePtr = &(entity->CreateAction(attributes[ATTRIBUTE_CLASS], attributes[ATTRIBUTE_NAME]));
		return true;
	}

	bool XmlParseHelperAction::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName)
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

		Action* currentEntity = sharedDataPtr->CurrentScopePtr->As<Action>();

		// test for action prescribed attributes.
		std::uint32_t numReservedPrescribedAttributes = Action::NUM_RESERVED_PRESCRIBED_ATTRIBUTES;
		for (std::uint32_t i = numReservedPrescribedAttributes; i < currentEntity->AuxiliaryBegin(); i++)
		{
			if (currentEntity->operator[](i).Size() < 1)
			{
				std::stringstream str;
				str << "Prescribed Attribute #" << i << " of instance " << currentEntity->Name() << " not initialized.";
				throw std::exception(str.str().c_str());
			}
		}

		sharedDataPtr->CurrentScopePtr = sharedDataPtr->CurrentScopePtr->GetParent();

		return true;
	}

	IXmlParseHelper* XmlParseHelperAction::Clone() const
	{
		return new XmlParseHelperAction();
	}
}
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
			throw std::exception("ClearScreen syntax for <entity>. Missing attribute: name");
		if (!attributes.ContainsKey(ATTRIBUTE_CLASS))
			throw std::exception("ClearScreen syntax for <entity>. Missing attribute: class");

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::ENTITY_START))
			throw std::exception("ClearScreen script syntax");
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		UNREFERENCED_PARAMETER(transitionToStateRouter);
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
		UNREFERENCED_PARAMETER(transitionToEntityEnd);
		assert(transitionToEntityEnd);
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		UNREFERENCED_PARAMETER(transitionToStateRouter);
		assert(transitionToStateRouter);

		Entity* currentEntity = sharedDataPtr->CurrentScopePtr->As<Entity>();
		assert(currentEntity != nullptr);
		// test for entity prescribed attributes.
		std::uint32_t numReservedPrescribedAttributes = Entity::NUM_RESERVED_PRESCRIBED_ATTRIBUTES;
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

	IXmlParseHelper* XmlParseHelperEntity::Clone() const
	{
		return new XmlParseHelperEntity();
	}
}

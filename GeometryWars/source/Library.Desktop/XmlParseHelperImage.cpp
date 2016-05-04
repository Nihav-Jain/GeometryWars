#include "pch.h"
#include "XmlParseHelperImage.h"
#include "Image.h"


namespace Library {
	const std::string XmlParseHelperImage::ELEMENT_NAME = "image";

	XmlParseHelperImage::XmlParseHelperImage()
	{
	}

	bool XmlParseHelperImage::StartElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & elementName, const Hashmap<std::string, std::string>& attributes)
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
		Image * image = new Image();
		image->SetName(attributes["name"]);
		parent->Adopt(Entity::ATTRIBUTE_ACTIONS, *image);
		sharedDataPtr->CurrentScopePtr = image;

		return true;
	}

	bool XmlParseHelperImage::EndElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & elementName)
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

	IXmlParseHelper * XmlParseHelperImage::Clone() const
	{
		return new XmlParseHelperImage();
	}
}
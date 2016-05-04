#include "pch.h"
#include "XmlParseHelperSprite.h"
#include "SpriteRenderer.h"
#include "Renderer.h"
#include "Entity.h"

namespace Library {
	const std::string XmlParseHelperSprite::ELEMENT_NAME = "sprite_renderer";
	const std::string XmlParseHelperSprite::ATTRIBUTE_NAME = "name";

	XmlParseHelperSprite::XmlParseHelperSprite()
	{
	}

	bool XmlParseHelperSprite::StartElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & elementName, const Hashmap<std::string, std::string>& attributes)
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

		// TODO: Remove singleton!!!!!!!!!!!!!!!!!!!! By Yuhsiang
		Scope* parent = sharedDataPtr->CurrentScopePtr;
		SpriteRenderer * sprite = new SpriteRenderer();
		sprite->SetName(attributes[ATTRIBUTE_NAME]);
		parent->Adopt(Entity::ATTRIBUTE_ACTIONS, *sprite);

		//Renderer::GetInstance()->AddRenderable(sprite);
		sharedDataPtr->CurrentScopePtr = sprite;

		return true;
	}

	bool XmlParseHelperSprite::EndElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & elementName)
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

	IXmlParseHelper * XmlParseHelperSprite::Clone() const
	{
		return new XmlParseHelperSprite();
	}
}

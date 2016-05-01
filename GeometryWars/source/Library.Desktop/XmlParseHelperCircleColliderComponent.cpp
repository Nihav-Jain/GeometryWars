#include "pch.h"
#include "XmlParseHelperCircleColliderComponent.h"
#include "CircleColliderComponent.h"

namespace Library
{
	XmlParseHelperCircleColliderComponent::XmlParseHelperCircleColliderComponent()
	{
		mElementName = "component";
		mDerivedActionClassName = "CircleColliderComponent";

		mRequiredAttributes.Clear();
		mRequiredAttributes.PushBack(ATTRIBUTE_NAME);
		mRequiredAttributes.PushBack(CircleColliderComponent::ATTRIBUTE_RADIUS);
		mRequiredAttributes.PushBack(CircleColliderComponent::ATTRIBUTE_CHANNEL);
		mRequiredAttributes.PushBack(CircleColliderComponent::ATTRIBUTE_ENABLED);
	}

	IXmlParseHelper* XmlParseHelperCircleColliderComponent::Clone() const
	{
		return new XmlParseHelperCircleColliderComponent();
	}

	void XmlParseHelperCircleColliderComponent::ParseActionAttributes(Action& currentAction, const Hashmap<std::string, std::string>& attributes)
	{
		currentAction[CircleColliderComponent::ATTRIBUTE_RADIUS] = attributes[CircleColliderComponent::ATTRIBUTE_RADIUS];
		currentAction[CircleColliderComponent::ATTRIBUTE_CHANNEL] = attributes[CircleColliderComponent::ATTRIBUTE_CHANNEL];
		currentAction[CircleColliderComponent::ATTRIBUTE_ENABLED] = attributes[CircleColliderComponent::ATTRIBUTE_ENABLED];
	}

}
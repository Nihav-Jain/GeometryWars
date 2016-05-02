#include "pch.h"
#include "XmlParseHelperAnimationState.h"

namespace Library
{
	XmlParseHelperAnimationState::XmlParseHelperAnimationState()
	{
		mElementName = "animation_state";
		mDerivedActionClassName = "AnimationState";

		mRequiredAttributes.Clear();
		mRequiredAttributes.PushBack(ATTRIBUTE_NAME);
	}

	IXmlParseHelper* XmlParseHelperAnimationState::Clone() const
	{
		return new XmlParseHelperAnimationState();
	}
}

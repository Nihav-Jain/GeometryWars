#include "pch.h"
#include "XmlParseHelperAnimationFrame.h"

namespace Library
{
	XmlParseHelperAnimationFrame::XmlParseHelperAnimationFrame()
	{
		mElementName = "animation_frame";
		mDerivedActionClassName = "AnimationFrame";

		mRequiredAttributes.Clear();
		mRequiredAttributes.PushBack(ATTRIBUTE_NAME);
	}

	IXmlParseHelper* XmlParseHelperAnimationFrame::Clone() const
	{
		return new XmlParseHelperAnimationFrame();
	}
}

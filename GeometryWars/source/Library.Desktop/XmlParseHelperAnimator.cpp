#include "pch.h"
#include "XmlParseHelperAnimator.h"

namespace Library
{
	XmlParseHelperAnimator::XmlParseHelperAnimator()
	{
		mElementName = "animator";
		mDerivedActionClassName = "Animator";

		mRequiredAttributes.Clear();
		mRequiredAttributes.PushBack(ATTRIBUTE_NAME);
	}

	IXmlParseHelper* XmlParseHelperAnimator::Clone() const
	{
		return new XmlParseHelperAnimator();
	}
}

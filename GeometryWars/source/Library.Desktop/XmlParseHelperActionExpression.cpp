#include "pch.h"
#include "XmlParseHelperActionExpression.h"

namespace Library
{
	XmlParseHelperActionExpression::XmlParseHelperActionExpression()
	{
		mElementName = "expression";
		mDerivedActionClassName = "ActionExpression";

		mRequiredAttributes.Clear();
		mRequiredAttributes.PushBack(ATTRIBUTE_NAME);
	}

	IXmlParseHelper* XmlParseHelperActionExpression::Clone() const
	{
		return new XmlParseHelperActionExpression();
	}

}

#include "pch.h"
#include "XmlParseHelperActionWhile.h"
#include "ActionWhileLoop.h"

namespace Library
{
	XmlParseHelperActionWhile::XmlParseHelperActionWhile()
	{
		mElementName = "while";
		mDerivedActionClassName = "ActionWhileLoop";

		mRequiredAttributes.Clear();
		mRequiredAttributes.PushBack(ATTRIBUTE_NAME);
	}

	IXmlParseHelper* XmlParseHelperActionWhile::Clone() const
	{
		return new XmlParseHelperActionWhile();
	}


#pragma region LoopBody

	XmlParseHelperActionWhile::XmlParseHelperActionWhileLoopBody::XmlParseHelperActionWhileLoopBody()
	{
		mElementName = ActionWhileLoop::ATTRIBUTE_LOOP;
		mDerivedActionClassName = "ActionList";
		mActionInstanceName = ActionWhileLoop::ATTRIBUTE_LOOP;

		mRequiredAttributes.Clear();
	}

	IXmlParseHelper* XmlParseHelperActionWhile::XmlParseHelperActionWhileLoopBody::Clone() const
	{
		return new XmlParseHelperActionWhile::XmlParseHelperActionWhileLoopBody();
	}

#pragma endregion

}

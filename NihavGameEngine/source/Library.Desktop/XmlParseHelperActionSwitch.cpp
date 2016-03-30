#include "pch.h"
#include "XmlParseHelperActionSwitch.h"
#include "Entity.h"
#include "ActionList.h"
#include "ActionListSwitch.h"

namespace Library
{

	//const std::string XmlParseHelperActionSwitch::CLASS_ACTION_SWITCH = "ActionListSwitch";

	XmlParseHelperActionSwitch::XmlParseHelperActionSwitch()
	{
		mElementName = "switch";
		mDerivedActionClassName = "ActionListSwitch";

		mRequiredAttributes.Clear();
		mRequiredAttributes.PushBack(ATTRIBUTE_NAME);
	}

	IXmlParseHelper* XmlParseHelperActionSwitch::Clone() const
	{
		return new XmlParseHelperActionSwitch();
	}

#pragma region ParseHelperSwitchCase

	XmlParseHelperActionSwitch::XmlParseHelperActionSwitchCase::XmlParseHelperActionSwitchCase()
	{
		mElementName = "case";
		mDerivedActionClassName = "ActionListSwitchCase";

		mRequiredAttributes.Clear();
		mRequiredAttributes.PushBack(ATTRIBUTE_NAME);
	}

	IXmlParseHelper* XmlParseHelperActionSwitch::XmlParseHelperActionSwitchCase::Clone() const
	{
		return new XmlParseHelperActionSwitchCase();
	}

#pragma endregion

}

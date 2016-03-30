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

	void XmlParseHelperActionSwitch::ParseActionAttributes(Action& action, const Hashmap<std::string, std::string>& attributes)
	{
		if (attributes.ContainsKey(ActionListSwitch::ATTRIBUTE_SWITCH_VALUE))
		{
			assert(action.Is(ActionListSwitch::TypeIdClass()));
			ActionListSwitch* actionSwitch = static_cast<ActionListSwitch*>(&action);
			(*actionSwitch)[ActionListSwitch::ATTRIBUTE_SWITCH_VALUE] = attributes[ActionListSwitch::ATTRIBUTE_SWITCH_VALUE];
		}
	}

#pragma region ParseHelperSwitchCase

	XmlParseHelperActionSwitch::XmlParseHelperActionSwitchCase::XmlParseHelperActionSwitchCase()
	{
		mElementName = "case";
		mDerivedActionClassName = "ActionListSwitchCase";

		mRequiredAttributes.Clear();
		mRequiredAttributes.PushBack(ATTRIBUTE_NAME);
		mRequiredAttributes.PushBack(ActionListSwitch::ActionListSwitchCase::ATTRIBUTE_CASE_VALUE);
	}

	IXmlParseHelper* XmlParseHelperActionSwitch::XmlParseHelperActionSwitchCase::Clone() const
	{
		return new XmlParseHelperActionSwitchCase();
	}

	void XmlParseHelperActionSwitch::XmlParseHelperActionSwitchCase::ParseActionAttributes(Action& action, const Hashmap<std::string, std::string>& attributes)
	{
		assert(action.Is(ActionListSwitch::ActionListSwitchCase::TypeIdClass()));
		ActionListSwitch::ActionListSwitchCase* caseAction = static_cast<ActionListSwitch::ActionListSwitchCase*>(&action);

		(*caseAction)[ActionListSwitch::ActionListSwitchCase::ATTRIBUTE_CASE_VALUE] = attributes[ActionListSwitch::ActionListSwitchCase::ATTRIBUTE_CASE_VALUE];

		if (attributes.ContainsKey(ActionListSwitch::ActionListSwitchCase::ATTRIBUTE_BREAK))
		{
			if (attributes[ActionListSwitch::ActionListSwitchCase::ATTRIBUTE_BREAK] == "true")
				caseAction->MustBreak = true;
			else
				caseAction->MustBreak = false;
		}

		if (attributes.ContainsKey(ActionListSwitch::ActionListSwitchCase::ATTRIBUTE_DEFAULT))
		{
			if (attributes[ActionListSwitch::ActionListSwitchCase::ATTRIBUTE_DEFAULT] == "true")
				caseAction->DefaultCase = true;
			else
				caseAction->DefaultCase = false;
		}
	}

#pragma endregion

}

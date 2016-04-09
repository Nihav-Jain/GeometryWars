#include "pch.h"
#include "XmlParseHelperActionEvent.h"
#include "ActionEvent.h"

namespace Library
{
	XmlParseHelperActionEvent::XmlParseHelperActionEvent()
	{
		mElementName = "event";
		mDerivedActionClassName = "ActionEvent";

		mRequiredAttributes.Clear();
		mRequiredAttributes.PushBack(ATTRIBUTE_NAME);
		mRequiredAttributes.PushBack(ActionEvent::ATTRIBUTE_SUBTYPE);
	}

	IXmlParseHelper* XmlParseHelperActionEvent::Clone() const
	{
		return new XmlParseHelperActionEvent();
	}

	void XmlParseHelperActionEvent::ParseActionAttributes(Action& currentAction, const Hashmap<std::string, std::string>& attributes)
	{
		currentAction[ActionEvent::ATTRIBUTE_SUBTYPE] = attributes[ActionEvent::ATTRIBUTE_SUBTYPE];
		if (attributes.ContainsKey(ActionEvent::ATTRIBUTE_DELAY))
		{
			currentAction[ActionEvent::ATTRIBUTE_DELAY] = attributes[ActionEvent::ATTRIBUTE_DELAY];
		}
	}

}

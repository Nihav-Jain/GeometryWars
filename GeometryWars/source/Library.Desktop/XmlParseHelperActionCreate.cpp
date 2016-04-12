#include "pch.h"
#include "XmlParseHelperActionCreate.h"
#include "ActionCreateAction.h"

namespace Library
{

	XmlParseHelperActionCreate::XmlParseHelperActionCreate()
	{
		mElementName = "createAction";
		mDerivedActionClassName = "ActionCreateAction";

		mRequiredAttributes.Clear();
		mRequiredAttributes.PushBack(ATTRIBUTE_NAME);
		mRequiredAttributes.PushBack(ActionCreateAction::ATTRIBUTE_PROTOTYPE_CLASS);
		mRequiredAttributes.PushBack(ActionCreateAction::ATTRIBUTE_PROTOTYPE_NAME);
	}

	IXmlParseHelper* XmlParseHelperActionCreate::Clone() const
	{
		return new XmlParseHelperActionCreate();
	}

	void XmlParseHelperActionCreate::ParseActionAttributes(Action& action, const Hashmap<std::string, std::string>& attributes)
	{
		action[ActionCreateAction::ATTRIBUTE_PROTOTYPE_CLASS] = attributes[ActionCreateAction::ATTRIBUTE_PROTOTYPE_CLASS];
		action[ActionCreateAction::ATTRIBUTE_PROTOTYPE_NAME] = attributes[ActionCreateAction::ATTRIBUTE_PROTOTYPE_NAME];
	}


}

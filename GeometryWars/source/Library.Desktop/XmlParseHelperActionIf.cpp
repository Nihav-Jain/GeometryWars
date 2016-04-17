#include "pch.h"
#include "XmlParseHelperActionIf.h"
#include "ActionIfThenElse.h"

namespace Library
{
	XmlParseHelperActionIf::XmlParseHelperActionIf()
	{
		mElementName = "if";
		mDerivedActionClassName = "ActionIfThenElse";

		mRequiredAttributes.Clear();
		mRequiredAttributes.PushBack(ATTRIBUTE_NAME);
	}

	IXmlParseHelper* XmlParseHelperActionIf::Clone() const
	{
		return new XmlParseHelperActionIf();
	}

#pragma region IfCondition

	XmlParseHelperActionIf::XmlParseHelperActionIfCondition::XmlParseHelperActionIfCondition()
	{
		mElementName = "condition";
		mDerivedActionClassName = "ActionExpression";
		mActionInstanceName = ActionIfThenElse::ATTRIBUTE_CONDITIONAL_EXP;

		mRequiredAttributes.Clear();
	}

	IXmlParseHelper* XmlParseHelperActionIf::XmlParseHelperActionIfCondition::Clone() const
	{
		return new XmlParseHelperActionIf::XmlParseHelperActionIfCondition();
	}

	void XmlParseHelperActionIf::XmlParseHelperActionIfCondition::PostXMLParsing(Action& currentAction)
	{
		std::string& expression = currentAction[ActionExpression::ATTRIBUTE_EXPRESSION].Get<std::string>();
		expression = ActionIfThenElse::ATTRIBUTE_CONDITION_RESULT + " = " + expression;
	}

#pragma endregion

#pragma region IfThen

	XmlParseHelperActionIf::XmlParseHelperActionIfThen::XmlParseHelperActionIfThen()
	{
		mElementName = ActionIfThenElse::ATTRIBUTE_THEN;
		mDerivedActionClassName = "ActionList";
		mActionInstanceName = ActionIfThenElse::ATTRIBUTE_THEN;

		mRequiredAttributes.Clear();
	}

	IXmlParseHelper* XmlParseHelperActionIf::XmlParseHelperActionIfThen::Clone() const
	{
		return new XmlParseHelperActionIf::XmlParseHelperActionIfThen();
	}

#pragma endregion

#pragma region IfElse

	XmlParseHelperActionIf::XmlParseHelperActionIfElse::XmlParseHelperActionIfElse()
	{
		mElementName = ActionIfThenElse::ATTRIBUTE_ELSE;
		mDerivedActionClassName = "ActionList";
		mActionInstanceName = ActionIfThenElse::ATTRIBUTE_ELSE;

		mRequiredAttributes.Clear();
	}

	IXmlParseHelper* XmlParseHelperActionIf::XmlParseHelperActionIfElse::Clone() const
	{
		return new XmlParseHelperActionIf::XmlParseHelperActionIfElse();
	}

#pragma endregion
}

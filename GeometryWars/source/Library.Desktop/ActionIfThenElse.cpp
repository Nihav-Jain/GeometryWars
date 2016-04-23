#include "pch.h"
#include "ActionIfThenElse.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionIfThenElse, ActionList);

	const std::string ActionIfThenElse::ATTRIBUTE_CONDITIONAL_EXP = "condition";
	const std::string ActionIfThenElse::ATTRIBUTE_THEN = "then";
	const std::string ActionIfThenElse::ATTRIBUTE_ELSE = "else";
	const std::string ActionIfThenElse::ATTRIBUTE_CONDITION_RESULT = "conditionResult";

	ActionIfThenElse::ActionIfThenElse() :
		mThen(nullptr), mElse(nullptr), mConditionExpression(nullptr)
	{
		AddInternalAttribute(ATTRIBUTE_CONDITION_RESULT, false, 1);
	}


	ActionIfThenElse::~ActionIfThenElse()
	{
	}

	void ActionIfThenElse::PostParsingProcess()
	{
		Action* helperAction = FindAction(ATTRIBUTE_CONDITIONAL_EXP);
		assert(helperAction != nullptr);
		mConditionExpression = helperAction->AssertiveAs<ActionExpression>();

		helperAction = FindAction(ATTRIBUTE_THEN);
		assert(helperAction != nullptr);
		mThen = helperAction->AssertiveAs<ActionList>();

		helperAction = FindAction(ATTRIBUTE_ELSE);
		if(helperAction != nullptr)
			mElse = helperAction->AssertiveAs<ActionList>();
	}

	void ActionIfThenElse::Update(WorldState& worldState)
	{
		worldState.action = mConditionExpression;
		mConditionExpression->Update(worldState);
		if ((*this)[ATTRIBUTE_CONDITION_RESULT].Get<bool>())
		{
			worldState.action = mThen;
			mThen->Update(worldState);
		}
		else if (mElse != nullptr)
		{
			worldState.action = mElse;
			mElse->Update(worldState);
		}
	}
}

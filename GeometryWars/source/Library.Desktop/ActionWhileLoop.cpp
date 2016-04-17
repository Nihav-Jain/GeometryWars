#include "pch.h"
#include "ActionWhileLoop.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionWhileLoop);

	const std::string ActionWhileLoop::ATTRIBUTE_CONDITIONAL_EXP = "condition";
	const std::string ActionWhileLoop::ATTRIBUTE_LOOP = "loop";
	const std::string ActionWhileLoop::ATTRIBUTE_CONDITION_RESULT = "conditionResult";

	ActionWhileLoop::ActionWhileLoop() :
		mLoopBody(nullptr), mConditionExpression(nullptr)
	{
		AddInternalAttribute(ATTRIBUTE_CONDITION_RESULT, false, 1);
	}

	void ActionWhileLoop::PostParsingProcess()
	{
		Action* helperAction = FindAction(ATTRIBUTE_CONDITIONAL_EXP);
		assert(helperAction != nullptr);
		mConditionExpression = helperAction->AssertiveAs<ActionExpression>();

		helperAction = FindAction(ATTRIBUTE_LOOP);
		assert(helperAction != nullptr);
		mLoopBody = helperAction->AssertiveAs<ActionList>();
	}

	void ActionWhileLoop::Update(WorldState& worldState)
	{
		worldState.action = mConditionExpression;
		mConditionExpression->Update(worldState);

		while ((*this)[ATTRIBUTE_CONDITION_RESULT].Get<bool>())
		{
			worldState.action = mLoopBody;
			mLoopBody->Update(worldState);

			worldState.action = mConditionExpression;
			mConditionExpression->Update(worldState);
		}
	}
}

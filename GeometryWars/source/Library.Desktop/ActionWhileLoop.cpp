#include "pch.h"
#include "ActionWhileLoop.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionWhileLoop, ActionList);

	const std::string ActionWhileLoop::ATTRIBUTE_CONDITIONAL_EXP = "condition";
	const std::string ActionWhileLoop::ATTRIBUTE_LOOP = "loop";
	const std::string ActionWhileLoop::ATTRIBUTE_CONDITION_RESULT = "conditionResult";

	ActionWhileLoop::ActionWhileLoop() :
		mLoopBody(nullptr), mConditionExpression(nullptr)
	{
		AddInternalAttribute(ATTRIBUTE_CONDITION_RESULT, false, 1);
	}

	ActionWhileLoop::ActionWhileLoop(const ActionWhileLoop& rhs) :
		ActionList::ActionList(rhs), mLoopBody(nullptr), mConditionExpression(nullptr)
	{}

	ActionWhileLoop::ActionWhileLoop(ActionWhileLoop&& rhs) :
		ActionList::ActionList(std::move(rhs)), mLoopBody(rhs.mLoopBody), mConditionExpression(rhs.mConditionExpression)
	{
		rhs.mLoopBody = nullptr;
		rhs.mConditionExpression = nullptr;
	}

	ActionWhileLoop& ActionWhileLoop::operator=(const ActionWhileLoop& rhs)
	{
		if (this != &rhs)
		{
			mLoopBody = nullptr;
			mConditionExpression = nullptr;

			ActionList::operator=(rhs);
		}
		return *this;
	}

	ActionWhileLoop& ActionWhileLoop::operator=(ActionWhileLoop&& rhs)
	{
		if (this != &rhs)
		{
			mLoopBody = rhs.mLoopBody;
			mConditionExpression = rhs.mConditionExpression;
			rhs.mLoopBody = nullptr;
			rhs.mConditionExpression = nullptr;

			ActionList::operator=(std::move(rhs));
		}
		return *this;
	}

	void ActionWhileLoop::BeginPlay(WorldState& worldState)
	{
		Action* helperAction = FindAction(ATTRIBUTE_CONDITIONAL_EXP);
		assert(helperAction != nullptr);
		mConditionExpression = helperAction->AssertiveAs<ActionExpression>();

		helperAction = FindAction(ATTRIBUTE_LOOP);
		assert(helperAction != nullptr);
		mLoopBody = helperAction->AssertiveAs<ActionList>();

		ActionList::BeginPlay(worldState);
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

	Scope* ActionWhileLoop::Clone(const Scope& rhs) const
	{
		ActionWhileLoop& action = *rhs.AssertiveAs<ActionWhileLoop>();
		return new ActionWhileLoop(action);
	}
}

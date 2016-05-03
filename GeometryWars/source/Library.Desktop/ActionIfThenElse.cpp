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

	ActionIfThenElse::ActionIfThenElse(const ActionIfThenElse& rhs) :
		ActionList::ActionList(rhs), mThen(rhs.mThen), mElse(rhs.mElse), mConditionExpression(rhs.mConditionExpression)
	{}

	ActionIfThenElse::ActionIfThenElse(ActionIfThenElse&& rhs) :
		ActionList::ActionList(std::move(rhs)), mThen(rhs.mThen), mElse(rhs.mElse), mConditionExpression(rhs.mConditionExpression)
	{
		rhs.mThen = nullptr;
		rhs.mElse = nullptr;
		rhs.mConditionExpression = nullptr;
	}

	ActionIfThenElse& ActionIfThenElse::operator=(const ActionIfThenElse& rhs)
	{
		if (this != &rhs)
		{
			mThen = rhs.mThen;
			mElse = rhs.mElse;
			mConditionExpression = rhs.mConditionExpression;
			ActionList::operator=(rhs);
		}
		return *this;
	}

	ActionIfThenElse& ActionIfThenElse::operator=(ActionIfThenElse&& rhs)
	{
		if (this != &rhs)
		{
			mThen = rhs.mThen;
			mElse = rhs.mElse;
			mConditionExpression = rhs.mConditionExpression;

			ActionList::operator=(std::move(rhs));

			rhs.mThen = nullptr;
			rhs.mElse = nullptr;
			rhs.mConditionExpression = nullptr;
		}
		return *this;
	}

	void ActionIfThenElse::BeginPlay(WorldState& worldState)
	{
		Action* helperAction = FindAction(ATTRIBUTE_CONDITIONAL_EXP);
		assert(helperAction != nullptr);
		mConditionExpression = helperAction->AssertiveAs<ActionExpression>();

		helperAction = FindAction(ATTRIBUTE_THEN);
		assert(helperAction != nullptr);
		mThen = helperAction->AssertiveAs<ActionList>();

		helperAction = FindAction(ATTRIBUTE_ELSE);
		if (helperAction != nullptr)
			mElse = helperAction->AssertiveAs<ActionList>();

		ActionList::BeginPlay(worldState);
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

	Scope* ActionIfThenElse::Clone(const Scope& rhs) const
	{
		ActionIfThenElse& action = *rhs.AssertiveAs<ActionIfThenElse>();
		return new ActionIfThenElse(action);
	}
}

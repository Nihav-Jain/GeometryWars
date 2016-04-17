#pragma once
#include "ActionList.h"
#include "ActionExpression.h"

namespace Library
{
	class ActionIfThenElse : public ActionList
	{
		RTTI_DECLARATIONS(ActionIfThenElse, ActionList);
	public:
		ActionIfThenElse();
		virtual ~ActionIfThenElse();

		ActionIfThenElse(const ActionIfThenElse& rhs) = delete;
		ActionIfThenElse& operator=(const ActionIfThenElse& rhs) = delete;

		virtual void PostParsingProcess() override;
		virtual void Update(WorldState& worldState) override;

		static const std::string ATTRIBUTE_CONDITIONAL_EXP;
		static const std::string ATTRIBUTE_THEN;
		static const std::string ATTRIBUTE_ELSE;
		static const std::string ATTRIBUTE_CONDITION_RESULT;

	private:
		ActionExpression* mConditionExpression;
		ActionList* mThen;
		ActionList* mElse;
	};

	CONCRETE_ACTION_FACTORY(ActionIfThenElse);
}


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
		virtual ~ActionIfThenElse() = default;

		ActionIfThenElse(const ActionIfThenElse& rhs);
		ActionIfThenElse(ActionIfThenElse&& rhs);

		ActionIfThenElse& operator=(const ActionIfThenElse& rhs);
		ActionIfThenElse& operator=(ActionIfThenElse&& rhs);

		virtual void BeginPlay(WorldState& worldState) override;
		virtual void Update(WorldState& worldState) override;
		virtual Scope* Clone(const Scope& rhs) const override;

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


#pragma once
#include "ActionList.h"
#include "ActionExpression.h"

namespace Library
{
	class ActionWhileLoop : public ActionList
	{
		RTTI_DECLARATIONS(ActionWhileLoop, ActionList);
	public:
		ActionWhileLoop();
		virtual ~ActionWhileLoop() = default;

		ActionWhileLoop(const ActionWhileLoop& rhs);
		ActionWhileLoop(ActionWhileLoop&& rhs);

		ActionWhileLoop& operator=(const ActionWhileLoop& rhs);
		ActionWhileLoop& operator=(ActionWhileLoop&& rhs);

		virtual void BeginPlay(WorldState& worldState) override;
		virtual void Update(WorldState& worldState) override;
		virtual Scope* Clone(const Scope& rhs) const override;

		static const std::string ATTRIBUTE_CONDITIONAL_EXP;
		static const std::string ATTRIBUTE_LOOP;
		static const std::string ATTRIBUTE_CONDITION_RESULT;

	private:
		ActionExpression* mConditionExpression;
		ActionList* mLoopBody;
	};

	CONCRETE_ACTION_FACTORY(ActionWhileLoop);
}


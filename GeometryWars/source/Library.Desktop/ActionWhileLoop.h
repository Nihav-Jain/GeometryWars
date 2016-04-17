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

		ActionWhileLoop(const ActionWhileLoop& rhs) = delete;
		ActionWhileLoop& operator=(const ActionWhileLoop& rhs) = delete;

		virtual void PostParsingProcess() override;
		virtual void Update(WorldState& worldState) override;

		static const std::string ATTRIBUTE_CONDITIONAL_EXP;
		static const std::string ATTRIBUTE_LOOP;
		static const std::string ATTRIBUTE_CONDITION_RESULT;

	private:
		ActionExpression* mConditionExpression;
		ActionList* mLoopBody;
	};

	CONCRETE_ACTION_FACTORY(ActionWhileLoop);
}


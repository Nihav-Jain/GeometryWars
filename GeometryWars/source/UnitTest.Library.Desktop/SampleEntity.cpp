#include "pch.h"
#include "SampleEntity.h"
#include "ActionExpression.h"

using namespace Library;

RTTI_DEFINITIONS(SampleEntity, Entity);

SampleEntity::SampleEntity()
{
	AddInternalAttribute("someInt", 0, 1);

	Action& action = Action::CreateAction("ActionExpression", "someExpression", *this, Entity::ATTRIBUTE_ACTIONS);
	action[ActionExpression::ATTRIBUTE_EXPRESSION] = "someInt = {10}";
	action.PostParsingProcess();
}

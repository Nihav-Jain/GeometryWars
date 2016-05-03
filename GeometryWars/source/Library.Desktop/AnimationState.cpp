#include "pch.h"
#include "AnimationState.h"

namespace Library
{
	RTTI_DEFINITIONS(AnimationState, ActionList);

	AnimationState::AnimationState()
	{
	}


	AnimationState::~AnimationState()
	{
	}
	Scope * AnimationState::Clone(const Scope & rhs) const
	{
		AnimationState& action = *rhs.AssertiveAs<AnimationState>();
		return new AnimationState(action);
	}
}

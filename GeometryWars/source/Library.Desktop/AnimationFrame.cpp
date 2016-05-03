#include "pch.h"
#include "AnimationFrame.h"

namespace Library
{
	RTTI_DEFINITIONS(AnimationFrame, Action);

	AnimationFrame::AnimationFrame()
	{
	}


	AnimationFrame::~AnimationFrame()
	{
	}

	Scope * AnimationFrame::Clone(const Scope & rhs) const
	{
		AnimationFrame& action = *rhs.AssertiveAs<AnimationFrame>();
		return new AnimationFrame(action);
	}
}

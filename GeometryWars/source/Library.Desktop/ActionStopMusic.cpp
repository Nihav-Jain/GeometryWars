#include "pch.h"
#include "ActionStopMusic.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionStopMusic, ActionList);

		const std::string ActionStopMusic::ATTRIBUTE_NAME = "musicName";

	ActionStopMusic::ActionStopMusic()
	{
		AddInternalAttribute(ATTRIBUTE_NAME, "", 0);

	}

	Scope * ActionStopMusic::Clone(const Scope & rhs) const
	{
		ActionStopMusic& entity = *rhs.AssertiveAs<ActionStopMusic>();
		return new ActionStopMusic(entity);
	}

	void ActionStopMusic::Update(WorldState & worldState)
	{
		worldState.world->GetAudioManager().StopMusic((*this)[ATTRIBUTE_NAME].Get<std::string>());
	}
}


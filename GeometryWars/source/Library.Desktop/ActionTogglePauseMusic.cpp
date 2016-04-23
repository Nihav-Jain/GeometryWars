#include "pch.h"
#include "ActionTogglePauseMusic.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionTogglePauseMusic)

		const std::string ActionTogglePauseMusic::ATTRIBUTE_NAME = "musicName";

	ActionTogglePauseMusic::ActionTogglePauseMusic()
	{
		AddInternalAttribute(ATTRIBUTE_NAME, "", 0);

	}

	void ActionTogglePauseMusic::Update(WorldState & worldState)
	{
		worldState.world->GetAudioManager().TogglePauseMusic((*this)[ATTRIBUTE_NAME].Get<std::string>());
	}
}


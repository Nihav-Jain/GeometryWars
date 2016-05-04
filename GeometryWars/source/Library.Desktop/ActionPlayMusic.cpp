#include "pch.h"
#include "ActionPlayMusic.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionPlayMusic, ActionList);

	const std::string ActionPlayMusic::ATTRIBUTE_NAME = "musicName";
	const std::string ActionPlayMusic::ATTRIBUTE_VOLUME = "volume";
	const std::string ActionPlayMusic::ATTRIBUTE_LOOP = "loop";

	ActionPlayMusic::ActionPlayMusic()
	{
		AddInternalAttribute(ATTRIBUTE_NAME, "", 0);
		AddInternalAttribute(ATTRIBUTE_VOLUME, 1.0f, 1);
		AddInternalAttribute(ATTRIBUTE_LOOP, 1, 1);
	}

	Scope * ActionPlayMusic::Clone(const Scope & rhs) const
	{
		ActionPlayMusic& entity = *rhs.AssertiveAs<ActionPlayMusic>();
		return new ActionPlayMusic(entity);
	}

	void ActionPlayMusic::Update(WorldState & worldState)
	{
			worldState.world->
			GetAudioManager().PlayMusic(
			(*this)[ATTRIBUTE_NAME].Get<std::string>(),
			(*this)[ATTRIBUTE_LOOP].Get<std::int32_t>(),
			(*this)[ATTRIBUTE_VOLUME].Get<std::float_t>());
	}
}

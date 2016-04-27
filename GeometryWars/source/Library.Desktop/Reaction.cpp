#include "pch.h"
#include "Reaction.h"

namespace Library
{
	RTTI_DEFINITIONS(Reaction, ActionList);

	Reaction::Reaction()
	{}

	void Reaction::Update(WorldState& worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
	}

}

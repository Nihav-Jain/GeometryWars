#include "pch.h"
#include "WorldState.h"

namespace Library
{
	WorldState::WorldState(const GameTime& gameTime) :
		world(nullptr), sector(nullptr), entity(nullptr), action(nullptr), mGameTime(&gameTime)
	{}

	WorldState::WorldState(WorldState&& rhs) :
		world(rhs.world), sector(rhs.sector), entity(rhs.entity), action(rhs.action), mGameTime(rhs.mGameTime)
	{
		rhs.world = nullptr;
		rhs.sector = nullptr;
		rhs.entity = nullptr;
		rhs.action = nullptr;
		rhs.mGameTime = nullptr;
	}

	WorldState& WorldState::operator=(WorldState&& rhs)
	{
		if (this != &rhs)
		{
			world = rhs.world;
			sector = rhs.sector;
			entity = rhs.entity;
			action = rhs.action;
			mGameTime = rhs.mGameTime;

			rhs.world = nullptr;
			rhs.sector = nullptr;
			rhs.entity = nullptr;
			rhs.action = nullptr;
			rhs.mGameTime = nullptr;
		}

		return *this;
	}

}

#include "pch.h"
#include "WorldState.h"

namespace Library
{
	WorldState::WorldState() :
		world(nullptr), sector(nullptr), entity(nullptr)
	{}

	WorldState::WorldState(WorldState&& rhs) :
		world(rhs.world), sector(rhs.sector), entity(rhs.entity)
	{
		rhs.world = nullptr;
		rhs.sector = nullptr;
		rhs.entity = nullptr;
	}

	WorldState& WorldState::operator=(WorldState&& rhs)
	{
		if (this != &rhs)
		{
			world = rhs.world;
			sector = rhs.sector;
			entity = rhs.entity;

			rhs.world = nullptr;
			rhs.sector = nullptr;
			rhs.entity = nullptr;
		}

		return *this;
	}



}

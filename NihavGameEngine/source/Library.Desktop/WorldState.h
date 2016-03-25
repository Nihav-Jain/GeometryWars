#pragma once

namespace Library
{
	class GameTime;

	class WorldState
	{
	public:
		WorldState();
		WorldState(const WorldState& rhs) = default;
		WorldState(WorldState&& rhs);
		~WorldState() = default;

		WorldState& operator=(const WorldState& rhs) = default;
		WorldState& operator=(WorldState&& rhs);

		const GameTime& GetGameTime() const;
		void SetGameTime(const GameTime& gameTime);

		class World* world;
		class Sector* sector;
		class Entity* entity;
	private:
		const GameTime* mGameTime;
	};

}


#pragma once

namespace Library
{
	class GameTime;

	/**
	 *	Represents the current "micro-state" of processing
	 *	Serves the role of passing data between various layers in the World
	 */
	class WorldState
	{
	public:
		/**
		 *	Constructor
		 *	@param constant reference to GameTime
		 */
		explicit WorldState(const GameTime& gameTime);

		/**
		 *	default copy constructor
		 */
		WorldState(const WorldState& rhs) = default;

		/**
		 *	move constructor
		 *	@param R value reference to the WorldState to be moved
		 */
		WorldState(WorldState&& rhs);

		/**
		 *	default destructor
		 */
		~WorldState() = default;

		/**
		 *	default copy assignment operator
		 */
		WorldState& operator=(const WorldState& rhs) = default;

		/**
		 *	move assignment operator
		 *	@param R value reference to the WorldState to be moved
		 *	@return reference to the WorldState the data was moved into
		 */
		WorldState& operator=(WorldState&& rhs);

		class World* world;
		class Sector* sector;
		class Entity* entity;
		class Action* action;
		const GameTime* mGameTime;
	};

}


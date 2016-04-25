#pragma once
#include "../../source/Library.Desktop/GameObject.h"

namespace Library
{
	class Enemy : public GameObject
	{
		RTTI_DECLARATIONS(Enemy, GameObject);

	public:

		Enemy();
		virtual ~Enemy() = default;

		void EnemyDeath(WorldState& worldState);

		virtual void OnOverlapBegin(const GameObject& other, WorldState& worldState) override;


	protected:

		//class Player* mPlayer;	// Cache the player? What about multiplayer?

	};

	CONCRETE_ENTITY_FACTORY(Enemy);
}


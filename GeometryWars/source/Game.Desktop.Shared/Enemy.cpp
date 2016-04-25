#include "pch.h"
#include "../../source/Library.Desktop/Game.h"
#include "Enemy.h"
#include "Player.h"

namespace Library
{
	RTTI_DEFINITIONS(Enemy, GameObject);

	Enemy::Enemy()
	{
	}

	void Enemy::EnemyDeath(WorldState & worldState)
	{
		MarkForDestroy(worldState);
	}

	void Enemy::OnOverlapBegin(const GameObject & other, WorldState & worldState)
	{
		Player* player = other.AssertiveAs<Player>();

		player->PlayerDeath(worldState);
		
		EnemyDeath(worldState);
	}

}
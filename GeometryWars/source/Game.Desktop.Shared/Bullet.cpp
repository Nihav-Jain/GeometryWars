#include "pch.h"
#include "../../source/Library.Desktop/Game.h"
#include "Bullet.h"
#include "Enemy.h"

namespace Library
{
	RTTI_DEFINITIONS(Bullet, GameObject);

	Bullet::Bullet()
	{
	}

	void Bullet::BulletDeath(WorldState & worldState)
	{
		MarkForDestroy(worldState);
	}

	void Bullet::OnOverlapBegin(const GameObject & other, WorldState & worldState)
	{
		Enemy* enemy = other.AssertiveAs<Enemy>();

		enemy->EnemyDeath(worldState);

		BulletDeath(worldState);
	}

}
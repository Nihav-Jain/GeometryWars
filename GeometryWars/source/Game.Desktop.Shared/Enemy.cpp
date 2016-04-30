#include "pch.h"
#include "../../source/Library.Desktop/Game.h"
#include "Enemy.h"
#include "Player.h"

namespace Library
{
	RTTI_DEFINITIONS(Enemy, GameObject);

	const std::string Enemy::ATTRIBUTE_VELOCITY = "velocity";

	Enemy::Enemy()
		: mVelocity()
	{
	}

	const glm::vec4 & Enemy::Velocity() const
	{
		return mVelocity;
	}

	void Enemy::SetVelocity(const glm::vec4 & velocity)
	{
		mVelocity = velocity;
	}

	void Enemy::EnemyDeath(WorldState & worldState)
	{
		MarkForDestroy(worldState);
	}

	void Enemy::BeginPlay(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
	}

	void Enemy::Update(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);

		mPosition += mVelocity;
	}

	void Enemy::OnDestroy(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
	}

	void Enemy::OnOverlapBegin(const GameObject & other, WorldState & worldState)
	{
		Player* player = other.AssertiveAs<Player>();

		player->PlayerDeath(worldState);
		
		EnemyDeath(worldState);
	}

}
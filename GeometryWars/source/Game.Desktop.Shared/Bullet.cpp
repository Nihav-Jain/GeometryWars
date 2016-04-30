#include "pch.h"
#include "../../source/Library.Desktop/Game.h"
#include "Bullet.h"
#include "Enemy.h"

namespace Library
{
	RTTI_DEFINITIONS(Bullet, GameObject);

	const std::string Bullet::ATTRIBUTE_VELOCITY = "velocity";
	const std::float_t Bullet::DEFAULT_SPEED = 20.0f;
	const glm::vec4 Bullet::DEFAULT_SIZE = glm::vec4(5.0f, 5.0f, 0.0f, 0.f);
	const std::string Bullet::DEFAULT_IMAGE = "Content/resource/mushroom.png";

	Bullet::Bullet()
		: mVelocity()
	{
	}

	const glm::vec4 & Bullet::Velocity() const
	{
		return mVelocity;
	}

	void Bullet::SetVelocity(const glm::vec4 & velocity)
	{
		mVelocity = velocity;
	}

	void Bullet::BulletDeath(WorldState & worldState)
	{
		MarkForDestroy(worldState);
	}

	void Bullet::BeginPlay(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
	}

	void Bullet::Update(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);

		mPosition += mVelocity;
	}

	void Bullet::OnDestroy(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
	}

	void Bullet::OnOverlapBegin(const GameObject & other, WorldState & worldState)
	{
		Enemy* enemy = other.AssertiveAs<Enemy>();

		enemy->EnemyDeath(worldState);

		BulletDeath(worldState);
	}

}
#include "pch.h"
#include "../../source/Library.Desktop/Game.h"
#include "../../source/Library.Desktop/PolygonRenderer.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Player.h"
#include "EnemySquare.h"


namespace Library
{
	RTTI_DEFINITIONS(EnemySquare, Enemy);

	const std::string EnemySquare::ATTRIBUTE_DODGE_CHANNEL = "dodgechannel";
	const std::string EnemySquare::ATTRIBUTE_DODGING = "isdodging";

	EnemySquare::EnemySquare()
		: mDodgeChannel(), mIsDodging(false)
	{
		AddExternalAttribute(ATTRIBUTE_DODGE_CHANNEL, 1, &mDodgeChannel);
		AddExternalAttribute(ATTRIBUTE_DODGING, 1, &mIsDodging);
	}

	EnemySquare::EnemySquare(const EnemySquare & rhs)
		: Enemy::Enemy(rhs), mDodgeChannel(rhs.mDodgeChannel), mIsDodging(rhs.mIsDodging)
	{
		ResetAttributePointers();
	}

	Scope * EnemySquare::Clone(const Scope & rhs) const
	{
		EnemySquare& entity = *rhs.AssertiveAs<EnemySquare>();
		return new EnemySquare(entity);
	}

	void EnemySquare::BeginPlay(WorldState & worldState)
	{
		WorldState savedWorldState = worldState;

		Bullet *bullet = GetSector()->CreateEntity(Bullet::TypeName(), "deadBullet").AssertiveAs<Bullet>();
		bullet->SetPosition(glm::vec4(99999.0f, 99999.0f, 99999.0f, 99999.0f));
		worldState.entity = GetSector()->FindEntity(Player::TypeName());
		bullet->BeginPlay(worldState);
		
		CircleColliderComponent::sCollidableEntitiesByChannel.Insert(mDodgeChannel, Bullet::TypeIdClass());

		worldState = savedWorldState;
		Enemy::BeginPlay(worldState);
	}

	void EnemySquare::Update(WorldState & worldState)
	{


		Enemy::Update(worldState);
	}

	void EnemySquare::OnOverlapBegin(const GameObject & other, const std::string& channel, WorldState & worldState)
	{
		Enemy::OnOverlapBegin(other, channel, worldState);

		if (mDodgeChannel == channel)
		{
			mIsDodging = true;

			glm::vec4 perpA, perpB;

			perpA.x = -other.AssertiveAs<Bullet>()->Velocity().y;
			perpA.y = other.AssertiveAs<Bullet>()->Velocity().x;

			perpA.x = other.AssertiveAs<Bullet>()->Velocity().y;
			perpA.y = -other.AssertiveAs<Bullet>()->Velocity().x;

			glm::vec4 newA = mPosition + perpA * MoveSpeed();
			glm::vec4 newB = mPosition + perpB * MoveSpeed();

			if (glm::distance(newA, other.Position()) > glm::distance(newB, other.Position()))
			{
				mVelocity = perpA;
			}
			else
			{
				mVelocity = perpB;
			}
		}
	}

	void EnemySquare::ResetAttributePointers()
	{
		(*this)[ATTRIBUTE_DODGE_CHANNEL].SetStorage(&mDodgeChannel, 1);
		(*this)[ATTRIBUTE_DODGING].SetStorage(&mIsDodging, 1);
	}

}
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
	const std::string EnemySquare::ATTRIBUTE_DODGE_ANGLE = "dodgeangle";
	const std::string EnemySquare::ATTRIBUTE_DODGING = "isdodging";

	EnemySquare::EnemySquare()
		: mDodgeChannel(), mDodgeAngle(), mIsDodging(false)
	{
		AddExternalAttribute(ATTRIBUTE_DODGE_CHANNEL, 1, &mDodgeChannel);
		AddExternalAttribute(ATTRIBUTE_DODGE_ANGLE, 1, &mDodgeAngle);
		AddExternalAttribute(ATTRIBUTE_DODGING, 1, &mIsDodging);	
	}

	EnemySquare::EnemySquare(const EnemySquare & rhs)
		: Enemy::Enemy(rhs), mDodgeChannel(rhs.mDodgeChannel), mDodgeAngle(rhs.mDodgeAngle), mIsDodging(rhs.mIsDodging)
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
		CircleColliderComponent::sCollidableEntitiesByChannel.Insert(mDodgeChannel, Bullet::TypeIdClass());

		Enemy::BeginPlay(worldState);
	}

	void EnemySquare::Update(WorldState & worldState)
	{
		mIsDodging = false;

		Enemy::Update(worldState);
	}

	void EnemySquare::OnOverlapBegin(const GameObject & other, const std::string& channel, WorldState & worldState)
	{
		Enemy::OnOverlapBegin(other, channel, worldState);

		if (mDodgeChannel == channel)
		{
			Bullet *bullet = other.AssertiveAs<Bullet>();
			glm::vec4 bulletDir = bullet->Direction();

			// Calculate angle between me and bullet direction
			glm::vec4 dir = glm::normalize(mPosition - other.Position());
			std::float_t dot = glm::dot(dir, bullet->Direction());
			std::float_t angle = acos(dot);
			if (angle < mDodgeAngle)
			{
				// Within dodging angle threshold, so move perpindicularly to bullet
				mIsDodging = true;

				glm::vec4 perpA, perpB;

				perpA.x = -bulletDir.y;
				perpA.y = bulletDir.x;

				perpB.x = bulletDir.y;
				perpB.y = -bulletDir.x;

				glm::vec4 newA = mPosition + perpA * MoveSpeed();
				glm::vec4 newB = mPosition + perpB * MoveSpeed();

				// Set direction along path that is closest
				if (glm::distance(newA, mPosition) > glm::distance(newB, mPosition))
				//if (glm::distance(newA, other.Position()) > glm::distance(newB, other.Position()))
				{
					SetDirection(perpA);
				}
				else
				{
					SetDirection(perpB);
				}
			}	
		}
	}

	void EnemySquare::ResetAttributePointers()
	{
		(*this)[ATTRIBUTE_DODGE_CHANNEL].SetStorage(&mDodgeChannel, 1);
		(*this)[ATTRIBUTE_DODGE_ANGLE].SetStorage(&mDodgeAngle, 1);
		(*this)[ATTRIBUTE_DODGING].SetStorage(&mIsDodging, 1);
	}

}
#include "pch.h"
#include "../../source/Library.Desktop/Game.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

namespace Library
{
	RTTI_DEFINITIONS(Player, GameObject);

	const std::string Player::ATTRIBUTE_PLAYERNUMBER = "player";
	const std::string Player::ATTRIBUTE_ATTACKSPEED = "attack speed";
	const std::string Player::ATTRIBUTE_CANATTACK = "can attack";
	const std::string Player::ATTRIBUTE_LIVES = "lives";
	const std::string Player::ATTRIBUTE_BOMBS = "bombs";
	const std::string Player::ATTRIBUTE_SHOOT = "shoot";
	const std::string Player::ATTRIBUTE_USEBOMB = "useBomb";
	const std::string Player::ATTRIBUTE_VELOCITY = "velocity";
	const std::string Player::ATTRIBUTE_HEADING = "heading";

	Player::Player()
		: mPlayerNumber(), mAttackSpeed(), mCanAttack(true), mShoot(false), mLives(3),
		  mBombCount(), mUseBomb(false), mVelocity(), mHeading()
	{
		AddExternalAttribute(ATTRIBUTE_PLAYERNUMBER, 1, &mPlayerNumber);
		AddExternalAttribute(ATTRIBUTE_ATTACKSPEED, 1, &mAttackSpeed);
		AddExternalAttribute(ATTRIBUTE_CANATTACK, 1, &mCanAttack);
		AddExternalAttribute(ATTRIBUTE_SHOOT, 1, &mShoot);
		AddExternalAttribute(ATTRIBUTE_LIVES, 1, &mLives);
		AddExternalAttribute(ATTRIBUTE_BOMBS, 1, &mBombCount);
		AddExternalAttribute(ATTRIBUTE_USEBOMB, 1, &mUseBomb);
		AddExternalAttribute(ATTRIBUTE_VELOCITY, 1, &mVelocity);
		AddExternalAttribute(ATTRIBUTE_HEADING, 1, &mHeading);
	}

	std::int32_t Player::PlayerNumber() const
	{
		return mPlayerNumber;
	}

	void Player::SetPlayerNumber(std::int32_t playerNumber)
	{
		mPlayerNumber = playerNumber;
	}

	std::float_t Player::AttackSpeed() const
	{
		return mAttackSpeed;
	}

	void Player::SetAttackSpeed(std::float_t attackSpeed)
	{
		mAttackSpeed = attackSpeed;
	}

	void Player::Shoot(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);

		if (mCanAttack)
		{
			mCanAttack = false;
			mShoot = false;

			// TODO: Spawn Bullet with velocity of mHeading

			// TODO: Reset cooldown by putting event into queue with mAttackSpeed delay
			//       and have a Reaction (in XML) to that event that sets mCanAttack to true
			mCanAttack = true;
		}
	}

	std::int32_t Player::Lives() const
	{
		return mLives;
	}

	void Player::SetLives(std::int32_t lives)
	{
		mLives = lives;
	}

	void Player::PlayerDeath(WorldState& worldState)
	{
		// Check for gameover
		if (mLives == 0)
		{
			// TODO: gameover
			MarkForDestroy(worldState);
		}
		else
		{
			--mLives;
		}
	}

	std::int32_t Player::Bombs() const
	{
		return mBombCount;
	}

	void Player::SetBombs(std::int32_t bombs)
	{
		mBombCount = bombs;
	}

	void Player::UseBomb(WorldState& worldState)
	{
		mUseBomb = false;

		if (mBombCount > 0)
		{
			Sector* mySector = GetSector();
			Vector<Entity*> enemies = mySector->GetAllEntitiesOfType(Enemy::TypeIdClass());
			std::int32_t numEnemies = enemies.Size();

			// Destroy all active enemies
			for (std::int32_t i = 0; i < numEnemies; ++i)
			{
				Enemy* enemy = enemies[i]->AssertiveAs<Enemy>();
				enemy->EnemyDeath(worldState);
			}

			// Use bomb, TODO: put on cooldown?
			--mBombCount;
		}
	}

	const glm::vec4 & Player::Velocity() const
	{
		return mVelocity;
	}

	void Player::SetVelocity(const glm::vec4 & velocity)
	{
		mVelocity = velocity;
	}

	const glm::vec4 & Player::Heading() const
	{
		return mHeading;
	}

	void Player::SetHeading(const glm::vec4 & heading)
	{
		mHeading = heading;
	}

	void Player::BeginPlay(WorldState & worldState)
	{
		GameObject::BeginPlay(worldState);
	}

	void Player::Update(WorldState & worldState)
	{
		GameObject::Update(worldState);

		// Shoot
		if (mShoot)
		{
			Shoot(worldState);
		}

		// Use a bomb
		if (mUseBomb)
		{
			UseBomb(worldState);
		}
	}

	void Player::OnDestroy(WorldState & worldState)
	{
		GameObject::OnDestroy(worldState);
	}

}
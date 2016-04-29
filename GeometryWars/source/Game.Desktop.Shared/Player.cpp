#include "pch.h"
#include "../../source/Library.Desktop/Game.h"
#include "Player.h"

namespace Library
{
	RTTI_DEFINITIONS(Player, GameObject);

	const std::string Player::ATTRIBUTE_ATTACKSPEED = "attack speed";
	const std::string Player::ATTRIBUTE_LIVES = "lives";
	const std::string Player::ATTRIBUTE_BOMBS = "bombs";

	Player::Player()
		: mAttackSpeed(), mLives(3), mBombs()
	{
		AddExternalAttribute(ATTRIBUTE_ATTACKSPEED, 1, &mAttackSpeed);
		AddExternalAttribute(ATTRIBUTE_LIVES, 1, &mLives);
		AddExternalAttribute(ATTRIBUTE_BOMBS, 1, &mBombs);
	}

	std::float_t Player::AttackSpeed() const
	{
		return mAttackSpeed;
	}

	void Player::SetAttackSpeed(std::float_t attackSpeed)
	{
		mAttackSpeed = attackSpeed;
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
		}
		else
		{
			--mLives;
			MarkForDestroy(worldState);
		}
	}

	std::int32_t Player::Bombs() const
	{
		return mBombs;
	}

	void Player::SetBombs(std::int32_t bombs)
	{
		mBombs = bombs;
	}

	void Player::UseBomb()
	{
		if (mBombs > 0)
		{
			// TODO: destroy all active enemies
		}
	}

}
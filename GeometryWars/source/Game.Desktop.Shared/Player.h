#pragma once
#include "../../source/Library.Desktop/GameObject.h"

namespace Library
{
	class Player : public GameObject
	{
		RTTI_DECLARATIONS(Player, GameObject);

	public:
		
		Player();
		virtual ~Player() = default;

		static const std::string ATTRIBUTE_ATTACKSPEED;
		static const std::string ATTRIBUTE_LIVES;
		static const std::string ATTRIBUTE_BOMBS;

		std::float_t AttackSpeed() const;
		void SetAttackSpeed(std::float_t attackSpeed);

		std::int32_t Lives() const;
		void SetLives(std::int32_t lives);
		void PlayerDeath();

		std::int32_t Bombs() const;
		void SetBombs(std::int32_t bombs);
		void UseBomb();

	protected:

		std::float_t mAttackSpeed;
		std::int32_t mLives;
		std::int32_t mBombs;

	};

	CONCRETE_ENTITY_FACTORY(Player);
}

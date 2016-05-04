#pragma once
#include "../../source/Library.Desktop/GameObject.h"

namespace Library
{
	class Player : public GameObject
	{
		RTTI_DECLARATIONS(Player, GameObject);

	public:
		
		Player();
		virtual ~Player();

		Player(const Player& rhs);

		static const std::string ATTRIBUTE_PLAYERNUMBER;
		static const std::string ATTRIBUTE_ATTACKSPEED;
		static const std::string ATTRIBUTE_CANATTACK;
		static const std::string ATTRIBUTE_LIVES;
		static const std::string ATTRIBUTE_DEAD;
		static const std::string ATTRIBUTE_MULTIPLIER;
		static const std::string ATTRIBUTE_SHOOT;
		static const std::string ATTRIBUTE_USEBOMB;
		static const std::string ATTRIBUTE_BOMBS;
		static const std::string ATTRIBUTE_HEADING;
		static const std::string ATTRIBUTE_CHANNEL;
		static const std::string ATTRIBUTE_SCOREBASE;

		void CheckScreenBounds();

		std::int32_t PlayerNumber() const;
		void SetPlayerNumber(std::int32_t playerNumber);

		std::int32_t AttackSpeed() const;
		void SetAttackSpeed(std::int32_t attackSpeed);
		void Shoot();

		const std::int32_t & Lives() const;
		void SetLives(std::int32_t lives);
		void PlayerDeath(WorldState& worldState);

		const std::int32_t Score() const;
		void AddScore(const std::int32_t & score);
		void SetScore(const std::int32_t & score);

		const std::int32_t Multiplier() const;
		void IncrementMultiplier();
		void ResetMultiplier();

		std::int32_t Bombs() const;
		void SetBombs(std::int32_t bombs);
		void UseBomb(WorldState& worldState);

		const glm::vec4 & Heading() const;
		void SetHeading(const glm::vec4 & heading);

		virtual Scope* Clone(const Scope& rhs) const override;
		virtual void BeginPlay(WorldState& worldState) override;
		virtual void Update(WorldState& worldState) override;
		virtual void OnDestroy(WorldState& worldState) override;

	protected:

		static bool sInitializedManagers;

		void CreateSpriteManagers() const;
		void InitSpriteManagers() const;

		std::int32_t mPlayerNumber;

		std::int32_t mAttackSpeed;
		bool mCanAttack;
		bool mShoot;
		glm::vec4 mHeading;

		std::int32_t mLives;
		bool mOutOfLives;
		std::string mCollisionChannel;
		std::int32_t mMultiplier;
		bool mUseBomb;
		std::int32_t mBombCount;
		

	private:
		std::chrono::milliseconds mShootTimer;

		void ResetAttributePointers();
		void DestroyAllEnemies(WorldState& worldState);

	};

	CONCRETE_ENTITY_FACTORY(Player);
}

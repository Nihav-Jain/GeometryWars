#include "pch.h"
#include "../../source/Library.Desktop/Game.h"
#include "../../source/Library.Desktop/SpriteRenderer.h"
#include "../../source/Library.Desktop/Image.h"
#include "../../source/Library.Desktop/Texture.h"
#include "../../source/Library.Desktop/InputManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Collectible.h"
#include "ScoreManager.h"
#include "LivesManager.h"
#include "BombManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

namespace Library
{
	RTTI_DEFINITIONS(Player, GameObject);

	const std::string Player::ATTRIBUTE_PLAYERNUMBER = "player";
	const std::string Player::ATTRIBUTE_ATTACKSPEED = "attackspeed";
	const std::string Player::ATTRIBUTE_CANATTACK = "canattack";
	const std::string Player::ATTRIBUTE_LIVES = "lives";
	const std::string Player::ATTRIBUTE_DEAD = "isdead";
	const std::string Player::ATTRIBUTE_MULTIPLIER = "multiplier";
	const std::string Player::ATTRIBUTE_BOMBS = "bombs";
	const std::string Player::ATTRIBUTE_SHOOT = "shoot";
	const std::string Player::ATTRIBUTE_USEBOMB = "useBomb";
	const std::string Player::ATTRIBUTE_HEADING = "heading";
	const std::string Player::ATTRIBUTE_CHANNEL = "playerchannel";
	const std::string Player::ATTRIBUTE_SCOREBASE = "scorebase";

	bool Player::sInitializedManagers = false;

	Player::Player()
		: mPlayerNumber(), mAttackSpeed(), mShootTimer(0), mCanAttack(true), mShoot(false), mLives(), mOutOfLives(false),
		  mMultiplier(1), mBombCount(), mUseBomb(false), mHeading(), mCollisionChannel()
	{
		AddExternalAttribute(ATTRIBUTE_PLAYERNUMBER, 1, &mPlayerNumber);
		AddExternalAttribute(ATTRIBUTE_ATTACKSPEED, 1, &mAttackSpeed);
		AddExternalAttribute(ATTRIBUTE_CANATTACK, 1, &mCanAttack);
		AddExternalAttribute(ATTRIBUTE_SHOOT, 1, &mShoot);
		AddExternalAttribute(ATTRIBUTE_LIVES, 1, &mLives);
		AddExternalAttribute(ATTRIBUTE_DEAD, 1, &mOutOfLives);
		AddExternalAttribute(ATTRIBUTE_MULTIPLIER, 1, &mMultiplier);
		AddExternalAttribute(ATTRIBUTE_BOMBS, 1, &mBombCount);
		AddExternalAttribute(ATTRIBUTE_USEBOMB, 1, &mUseBomb);
		AddExternalAttribute(ATTRIBUTE_HEADING, 1, &mHeading);
		AddExternalAttribute(ATTRIBUTE_CHANNEL, 1, &mCollisionChannel);

		AddInternalAttribute(ATTRIBUTE_SCOREBASE, 10);

		CreateSpriteManagers();
	}

	Player::~Player()
	{
		ScoreManager::DeleteInstance();
		LivesManager::DeleteInstance();
		BombManager::DeleteInstance();

		sInitializedManagers = false;
	}

	Player::Player(const Player & rhs)
		: GameObject::GameObject(rhs), mPlayerNumber(rhs.mPlayerNumber), mAttackSpeed(rhs.mAttackSpeed), mShootTimer(rhs.mShootTimer), mCanAttack(rhs.mCanAttack),
		mShoot(rhs.mShoot), mLives(rhs.mLives), mMultiplier(rhs.mMultiplier), mBombCount(rhs.mBombCount), mUseBomb(rhs.mUseBomb),
		mHeading(rhs.mHeading), mCollisionChannel(rhs.mCollisionChannel)
	{
		ResetAttributePointers();
	}

	std::int32_t Player::PlayerNumber() const
	{
		return mPlayerNumber;
	}

	void Player::SetPlayerNumber(std::int32_t playerNumber)
	{
		mPlayerNumber = playerNumber;
	}

	std::int32_t Player::AttackSpeed() const
	{
		return mAttackSpeed;
	}

	void Player::SetAttackSpeed(std::int32_t attackSpeed)
	{
		mAttackSpeed = attackSpeed;
	}

	void Player::Shoot()
	{
		mShootTimer = std::chrono::milliseconds(mAttackSpeed);
		mShoot = false;
	}

	const std::int32_t & Player::Lives() const
	{
		return mLives;
	}

	void Player::SetLives(std::int32_t lives)
	{
		mLives = lives;
		mOutOfLives = (mLives == 0);
		LivesManager::GetInstance()->SetValue(mLives);
	}

	void Player::PlayerDeath(WorldState& worldState)
	{
		UNREFERENCED_PARAMETER(worldState);

		// Change life counter
		SetLives(mLives - 1);
		ResetMultiplier();
		OutputDebugStringA("Player Hit!");
		
		// Destroy all active enemies
		DestroyAllEnemies(worldState, true);
		ResetMultiplier();

		// Check for gameover
		if (mLives <= 0)
		{
			OutputDebugStringA("Player is Dead!");
		}
	}

	const std::int32_t Player::Score() const
	{
		return ScoreManager::GetInstance()->GetValue();
	}

	void Player::AddScore(const std::int32_t & score)
	{
		std::int32_t scoreWMultiplier = score * mMultiplier;
		ScoreManager::GetInstance()->AddValue(scoreWMultiplier);

		OutputDebugStringA("Score = ");
		OutputDebugStringA(std::to_string(ScoreManager::GetInstance()->GetValue()).c_str());
		OutputDebugStringA("\n");
	}

	void Player::SetScore(const std::int32_t & score)
	{
		ScoreManager::GetInstance()->SetValue(score);

		OutputDebugStringA("Score = ");
		OutputDebugStringA(std::to_string(ScoreManager::GetInstance()->GetValue()).c_str());
		OutputDebugStringA("\n");
	}

	const std::int32_t Player::Multiplier() const
	{
		return mMultiplier;
	}

	void Player::IncrementMultiplier()
	{
		mMultiplier++;
	}

	void Player::ResetMultiplier()
	{
		mMultiplier = 1;
	}

	std::int32_t Player::Bombs() const
	{
		return mBombCount;
	}

	void Player::SetBombs(std::int32_t bombs)
	{
		mBombCount = bombs;
		BombManager::GetInstance()->SetValue(mBombCount);
	}

	void Player::UseBomb(WorldState& worldState)
	{
		mUseBomb = false;

		if (mBombCount > 0)
		{
			DestroyAllEnemies(worldState, false);

			--mBombCount;
			BombManager::GetInstance()->SetValue(mBombCount);
		}
	}

	const glm::vec4 & Player::Heading() const
	{
		return mHeading;
	}

	void Player::SetHeading(const glm::vec4 & heading)
	{
		mHeading = heading;
	}

	Scope * Player::Clone(const Scope & rhs) const
	{
		Player& entity = *rhs.AssertiveAs<Player>();
		return new Player(entity);
	}

	void Player::BeginPlay(WorldState & worldState)
	{
		mHeading = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

		GameObject::BeginPlay(worldState);

		InitSpriteManagers();
	}

	void Player::Update(WorldState & worldState)
	{
		// Prevent moving out of bounds
		CheckScreenBounds();

		// Shoot
		if (mShoot)
		{
			Shoot();
		}

		// Update cooldown on shooting
		if (mShootTimer <= std::chrono::milliseconds::zero())
		{
			mCanAttack = true;
		}
		else
		{
			mShootTimer -= worldState.mGameTime->ElapsedGameTime();
		}

		// Use a bomb
		if (mUseBomb)
		{
			UseBomb(worldState);
		}

		GameObject::Update(worldState);
	}

	void Player::CheckScreenBounds()
	{
		if (mPosition.x > (mWorldWidth / 2.0f) - mScale.x)
			mPosition.x = (mWorldWidth / 2.0f) - mScale.x;
		else if (mPosition.x < (-mWorldWidth / 2.0f) + mScale.x)
			mPosition.x = (-mWorldWidth / 2.0f) + mScale.x;

		if (mPosition.y > (mWorldHeight / 2.0f) - mScale.y)
			mPosition.y = (mWorldHeight / 2.0f) - mScale.y;
		else if (mPosition.y < (-mWorldHeight / 2.0f) + mScale.y)
			mPosition.y = (-mWorldHeight / 2.0f) + mScale.y;
	}

	void Player::OnDestroy(WorldState & worldState)
	{
		sInitializedManagers = false;

		ScoreManager::GetInstance()->CleanupSprites();
		LivesManager::GetInstance()->CleanupSprites();
		BombManager::GetInstance()->CleanupSprites();

		GameObject::OnDestroy(worldState);
	}

	void Player::CreateSpriteManagers() const
	{
		ScoreManager::CreateInstance();
		LivesManager::CreateInstance();
		BombManager::CreateInstance();
	}

	void Player::InitSpriteManagers() const
	{
		if (!sInitializedManagers)
		{
			sInitializedManagers = true;

			ScoreManager* score = ScoreManager::GetInstance();
			score->SetData(0, 10, 40, 200, 315, 10, false, "Content//resource//", "digits//", ".png");
			score->SetNumberBase(Find(ATTRIBUTE_SCOREBASE)->Get<std::int32_t>());
			score->Init();
			score->RefreshSprites();

			LivesManager* lives = LivesManager::GetInstance();
			lives->SetData(mLives, mLives, 30, -110, 315, -5, false, "Content//resource//", "", ".png");
			lives->Init();
			lives->RefreshSprites();

			BombManager* bomb = BombManager::GetInstance();
			bomb->SetData(mBombCount, mBombCount, 30, 30, 315, -5, true, "Content//resource//", "", ".png");
			bomb->Init();
			bomb->RefreshSprites();
		}
	}

	void Player::ResetAttributePointers()
	{
		(*this)[ATTRIBUTE_PLAYERNUMBER].SetStorage(&mPlayerNumber, 1);
		(*this)[ATTRIBUTE_ATTACKSPEED].SetStorage(&mAttackSpeed, 1);
		(*this)[ATTRIBUTE_CANATTACK].SetStorage(&mCanAttack, 1);
		(*this)[ATTRIBUTE_SHOOT].SetStorage(&mShoot, 1);
		(*this)[ATTRIBUTE_LIVES].SetStorage(&mLives, 1);
		(*this)[ATTRIBUTE_DEAD].SetStorage(&mOutOfLives, 1);
		(*this)[ATTRIBUTE_MULTIPLIER].SetStorage(&mMultiplier, 1);
		(*this)[ATTRIBUTE_BOMBS].SetStorage(&mBombCount, 1);
		(*this)[ATTRIBUTE_USEBOMB].SetStorage(&mUseBomb, 1);
		(*this)[ATTRIBUTE_HEADING].SetStorage(&mHeading, 1);
		(*this)[ATTRIBUTE_CHANNEL].SetStorage(&mCollisionChannel, 1);
	}

	void Player::DestroyAllEnemies(WorldState& worldState, bool destroyCollectibles)
	{
		Sector* mySector = GetSector();
		Vector<Entity*> enemies = mySector->GetAllEntitiesOfType(Enemy::TypeIdClass());
		std::int32_t numEnemies = enemies.Size();

		// Destroy all active enemies
		for (std::int32_t i = 0; i < numEnemies; ++i)
		{
			Enemy* enemy = enemies[i]->AssertiveAs<Enemy>();
			enemy->SetCanSpawnCollectible(!destroyCollectibles);

			if (enemy->Name() != "EnemySpawner")
			{
				enemy->EnemyDeath(worldState, true);
			}
		}
		
		if (destroyCollectibles)
		{
			Vector<Entity*> collectibles = mySector->GetAllEntitiesOfType(Collectible::TypeIdClass());
			std::int32_t numCollectibles = collectibles.Size();
			for (std::int32_t i = 0; i < numCollectibles; ++i)
			{
				Collectible* collectible = collectibles[i]->AssertiveAs<Collectible>();
				collectible->SetIsCollected(true);
			}
		}
	}

}
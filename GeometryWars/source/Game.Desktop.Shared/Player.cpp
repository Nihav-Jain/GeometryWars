#include "pch.h"
#include "../../source/Library.Desktop/Game.h"
#include "../../source/Library.Desktop/SpriteRenderer.h"
#include "../../source/Library.Desktop/Image.h"
#include "../../source/Library.Desktop/Texture.h"
#include "../../source/Library.Desktop/InputManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

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
	const std::string Player::ATTRIBUTE_BOMBS = "bombs";
	const std::string Player::ATTRIBUTE_SHOOT = "shoot";
	const std::string Player::ATTRIBUTE_USEBOMB = "useBomb";
	const std::string Player::ATTRIBUTE_VELOCITY = "velocity";
	const std::string Player::ATTRIBUTE_HEADING = "heading";
	const std::string Player::ATTRIBUTE_CHANNEL = "playerchannel";

	Player::Player()
		: mPlayerNumber(), mAttackSpeed(), mCanAttack(true), mShoot(false), mLives(3),
		  mBombCount(), mUseBomb(false), mVelocity(), mHeading(), mCollisionChannel()
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
		AddExternalAttribute(ATTRIBUTE_CHANNEL, 1, &mCollisionChannel);
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

	void Player::Shoot(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);

		if (mCanAttack)
		{
			mCanAttack = false;
			mShoot = false;

			// TODO: Load this prototype from an xml file
			//Bullet* newBullet = GetSector()->CreateEntity(Bullet::TypeName(), "bullet").AssertiveAs<Bullet>();
			//newBullet->SetPosition(mPosition);
			//newBullet->SetVelocity(mHeading * Bullet::DEFAULT_SPEED);
			//SpriteRenderer *bulletRenderer = Action::CreateAction(SpriteRenderer::TypeName(), "bulletRenderer", *newBullet, Entity::ATTRIBUTE_ACTIONS).AssertiveAs<SpriteRenderer>();
			//Image *bulletImage = bulletRenderer->CreateAction(Image::TypeName(), "image", *bulletRenderer, ActionList::ATTRIBUTE_ACTIONS).AssertiveAs<Image>();
			//bulletImage->SetPath(Bullet::DEFAULT_IMAGE);
			//bulletImage->SetSize(Bullet::DEFAULT_SIZE);
			//Renderer::GetInstance()->AddRenderable(bulletRenderer);

			// TODO: Reset cooldown by putting event into queue with mAttackSpeed delay
			//       and have a Reaction (in XML) to that event that sets mCanAttack to true
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
		UNREFERENCED_PARAMETER(worldState);

		// Check for gameover
		if (mLives <= 0)
		{
			// TODO: gameover
			//MarkForDestroy(worldState);
			OutputDebugStringA("Player is Dead!");
		}
		else
		{
			--mLives;
			OutputDebugStringA("Player Hit!");
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

		mHeading = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	}

	void Player::Update(WorldState & worldState)
	{
		GameObject::Update(worldState);

		// Update heading with rotation
		mHeading.x = -sin(mRotation.z);
		mHeading.y = cos(mRotation.z);
		mHeading = glm::normalize(mHeading);

		// Prevent moving out of bounds
		if (mPosition.x > mWorldWidth / 2.0f)
			mPosition.x = mWorldWidth / 2.0f;
		else if (mPosition.x < -mWorldWidth / 2.0f)
			mPosition.x = -mWorldWidth / 2.0f;

		if (mPosition.y > mWorldHeight / 2.0f)
			mPosition.y = mWorldHeight / 2.0f;
		else if (mPosition.y < -mWorldHeight / 2.0f)
			mPosition.y = -mWorldHeight / 2.0f;

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
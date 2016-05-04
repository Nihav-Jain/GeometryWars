#include "pch.h"
#include "GameObject.h"
#include "ActionExpression.h"

namespace Library
{
	RTTI_DEFINITIONS(GameObject, Entity);

	const std::string GameObject::ATTRIBUTE_POSITION = "position";
	const std::string GameObject::ATTRIBUTE_ROTATION = "rotation";
	const std::string GameObject::ATTRIBUTE_SCALE = "scale";
	const std::string GameObject::ATTRIBUTE_DIRECTION = "direction";
	const std::string GameObject::ATTRIBUTE_MOVESPEED = "speed";

	GameObject::GameObject()
		: mPosition(), mRotation(), mScale(1.0f), mDirection(), mMoveSpeed(), mWorldWidth(), mWorldHeight()
	{
		AddExternalAttribute(ATTRIBUTE_POSITION, 1, &mPosition);
		AddExternalAttribute(ATTRIBUTE_ROTATION, 1, &mRotation);
		AddExternalAttribute(ATTRIBUTE_SCALE, 1, &mScale);
		AddExternalAttribute(ATTRIBUTE_DIRECTION, 1, &mDirection);
		AddExternalAttribute(ATTRIBUTE_MOVESPEED, 1, &mMoveSpeed);
	}

	GameObject::GameObject(const GameObject & rhs) :
		Entity::Entity(rhs), mPosition(rhs.mPosition), mRotation(rhs.mRotation), mScale(rhs.mScale), mDirection(rhs.mDirection), mMoveSpeed(rhs.mMoveSpeed),
		mWorldWidth(rhs.mWorldWidth), mWorldHeight(rhs.mWorldHeight)
	{
		ResetAttributePointers();
	}

	const glm::vec4 & GameObject::Position() const
	{
		return mPosition;
	}

	void GameObject::SetPosition(const glm::vec4 & position)
	{
		mPosition = position;
	}

	const glm::vec4 & GameObject::Rotation() const
	{
		return mRotation;
	}

	void GameObject::SetRotation(const glm::vec4 & rotation)
	{
		mRotation = rotation;
	}

	const glm::vec4 & GameObject::Scale() const
	{
		return mScale;
	}

	void GameObject::SetScale(const glm::vec4 & scale)
	{
		mScale = scale;
	}

	const glm::vec4 & GameObject::Direction() const
	{
		return mDirection;
	}

	void GameObject::SetDirection(const glm::vec4 & direction)
	{
		mDirection = direction;
	}

	const std::float_t & GameObject::MoveSpeed() const
	{
		return mMoveSpeed;
	}

	void GameObject::SetMoveSpeed(const std::float_t & moveSpeed)
	{
		mMoveSpeed = moveSpeed;
	}

	Action* GameObject::GetComponent(const std::string & typeName) const
	{		
		return FindAction(typeName);
	}

	bool GameObject::HasComponent(const std::string & typeName) const
	{
		return (GetComponent(typeName) != nullptr);
	}

	Scope * GameObject::Clone(const Scope & rhs) const
	{
		GameObject& entity = *rhs.AssertiveAs<GameObject>();
		return new GameObject(entity);
	}

	void GameObject::BeginPlay(WorldState & worldState)
	{
		Entity::BeginPlay(worldState);

		mWorldWidth = worldState.world->GetWidth();
		mWorldHeight = worldState.world->GetHeight();
	}

	void GameObject::Update(WorldState & worldState)
	{
		Entity::Update(worldState);
	}

	void GameObject::OnDestroy(WorldState & worldState)
	{
		Entity::OnDestroy(worldState);
	}

	void GameObject::OnOverlapBegin(const GameObject & other, const std::string& channel, WorldState& worldState)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(channel);
		UNREFERENCED_PARAMETER(worldState);
	}

	void GameObject::ResetAttributePointers()
	{
		(*this)[ATTRIBUTE_POSITION].SetStorage(&mPosition, 1);
		(*this)[ATTRIBUTE_ROTATION].SetStorage(&mRotation, 1);
		(*this)[ATTRIBUTE_SCALE].SetStorage(&mScale, 1);
		(*this)[ATTRIBUTE_DIRECTION].SetStorage(&mDirection, 1);
		(*this)[ATTRIBUTE_MOVESPEED].SetStorage(&mMoveSpeed, 1);
	}
}



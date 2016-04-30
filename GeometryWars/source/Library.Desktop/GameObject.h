#pragma once
#include "Entity.h"

namespace Library
{
	class GameObject :	public Entity
	{
		RTTI_DECLARATIONS(GameObject, Entity);

	public:

		GameObject();
		virtual ~GameObject() = default;

		const glm::vec4& Position() const;
		void SetPosition(const glm::vec4& position);

		const glm::vec4& Rotation() const;
		void SetRotation(const glm::vec4& rotation);

		const glm::vec4& Scale() const;
		void SetScale(const glm::vec4& scale);

		const std::float_t& MoveSpeed() const;
		void SetMoveSpeed(const std::float_t& moveSpeed);

		Action* GetComponent(const std::string& typeName) const;
		bool HasComponent(const std::string& typeName) const;

		virtual void BeginPlay(WorldState& worldState) override;
		virtual void Update(WorldState& worldState) override;
		virtual void OnDestroy(WorldState& worldState) override;
		virtual void OnOverlapBegin(const GameObject& other, WorldState& worldState);

		static const std::string ATTRIBUTE_POSITION;
		static const std::string ATTRIBUTE_ROTATION;
		static const std::string ATTRIBUTE_SCALE;
		static const std::string ATTRIBUTE_MOVESPEED;

	protected:
		glm::vec4 mPosition, mRotation, mScale;
		std::float_t mMoveSpeed;

	};

	CONCRETE_ENTITY_FACTORY(GameObject);

}
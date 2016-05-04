#pragma once
#include "Entity.h"

namespace Library
{
	/**
	*	Base class for entities within the game.
	*	@attribute position
	*	@attribute rotation
	*	@attribute scale
	*	@attribute direction
	*	@attribute speed
	*	@inherits Entity
	*/
	class GameObject :	public Entity
	{
		RTTI_DECLARATIONS(GameObject, Entity);

	public:

		/**
		*	Constructor - initializes member variables and declares prescribed attributes
		*/
		GameObject();

		/**
		*	Destructor - default
		*/
		virtual ~GameObject() = default;

		/**
		*	Copy Constructor
		*/
		GameObject(const GameObject& rhs);

		/**
		*	Accessor for this object's position
		*/
		const glm::vec4& Position() const;

		/**
		*	Mutator for this object's position
		*/
		void SetPosition(const glm::vec4& position);

		/**
		*	Accessor for this object's rotation
		*/
		const glm::vec4& Rotation() const;

		/**
		*	Mutator for this object's rotation
		*/
		void SetRotation(const glm::vec4& rotation);

		/**
		*	Accessor for this object's scale
		*/
		const glm::vec4& Scale() const;

		/**
		*	Mutator for this object's scale
		*/
		void SetScale(const glm::vec4& scale);

		/**
		*	Accessor for this object's direction
		*/
		const glm::vec4& Direction() const;

		/**
		*	Mutator for this object's direction
		*/
		void SetDirection(const glm::vec4& direction);

		/**
		*	Accessor for this object's move speed
		*/
		const std::float_t& MoveSpeed() const;

		/**
		*	Mutator for this object's move speed
		*/
		void SetMoveSpeed(const std::float_t& moveSpeed);

		/**
		*	Returns a given component, if contained within this object.
		*	@param const string ref of class type name
		*	@return ptr to the component action (null if not found)
		*/
		Action* GetComponent(const std::string& typeName) const;

		/**
		*	Checks if the object has a component by the given class type name.
		*	@param const string ref of class type name
		*	@return bool (true if found, otherwise false)
		*/
		bool HasComponent(const std::string& typeName) const;

		virtual Scope* Clone(const Scope& rhs) const override;
		virtual void BeginPlay(WorldState& worldState) override;

		/**
		*	Checks if the object is outside of the world boundaries
		*	executes the matching case action list
		*	executes the default case if there are no matching cases
		*	@param reference to the world state
		*/
		virtual void Update(WorldState& worldState) override;

		virtual void OnDestroy(WorldState& worldState) override;
		virtual void OnOverlapBegin(const GameObject& other, const std::string& channel, WorldState& worldState);

		static const std::string ATTRIBUTE_POSITION;
		static const std::string ATTRIBUTE_ROTATION;
		static const std::string ATTRIBUTE_SCALE;
		static const std::string ATTRIBUTE_DIRECTION;
		static const std::string ATTRIBUTE_MOVESPEED;

	protected:
		glm::vec4 mPosition, mRotation, mScale, mDirection;
		std::float_t mMoveSpeed;

		std::int32_t mWorldWidth, mWorldHeight;

	private:
		void ResetAttributePointers();
	};

	CONCRETE_ENTITY_FACTORY(GameObject);

}
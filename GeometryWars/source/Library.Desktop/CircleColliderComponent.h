#pragma once
#include "Action.h"

namespace Library
{
	/**
	 *	Any Entity with this action becomes collidable
	 *	@grammar <component class="CircleColliderComponent" name="CircleColliderComponent">
	 *	@attribute name
	 *	@attribute class
	 *	@inherits Action
	 */
	class CircleColliderComponent final : public Action
	{
		RTTI_DECLARATIONS(CircleColliderComponent, Action);

	public:

		/**
		 *	Constructor - initializes member variables and declares prescribed attributes
		 */
		CircleColliderComponent();

		/**
		 *	dedault destructor
		 */
		~CircleColliderComponent() = default;

		/**
		 *	copy Constructor
		 */
		CircleColliderComponent(const CircleColliderComponent& rhs);

		/**
		 *	getter for the radius of the collider
		 *	@return float value representing radius
		 */
		const std::float_t& Radius() const;

		/**
		 *	setter for the radius of the collider
		 *	@param new radius
		 */
		void SetRadius(const std::float_t& radius);

		/**
		 *	getter for the status of the collider
		 *	@return true of enabled, false if disable
		 */
		bool IsEnabled() const;

		/**
		 *	setter for the status of the collider
		 *	@param new status of collider
		 */
		void SetEnabled(bool isEnabled);

		/**
		 *	getter for the GameObject which owns this component
		 *	@return const referemce to owner GameObject
		 */
		const GameObject& Owner() const;

		/**
		 *	Will be called by the parent GameObject's Update method every frame
		 *	Should be used to implement the main logic of a apecialized Action / Component
		 *	Pure virual function, needs to be implemented by the children
		 *	@param reference to the WorldState
		 */
		virtual void Update(WorldState& worldState) override;

		/**
		 *	virtual copy constructor
		 *	@return pointer to newly copy constructed CircleColliderComponent
		 */
		virtual Scope* Clone(const Scope& rhs) const override;

		/**
		 *	Sets the owner GameObject and the collision channel
		 *	@override BeginPlay
		 *	@param world state
		 */
		virtual void BeginPlay(WorldState& worldState) override;

		static const std::string ATTRIBUTE_RADIUS;
		static const std::string ATTRIBUTE_ENABLED;
		static const std::string ATTRIBUTE_CHANNEL;

		static Hashmap<std::string, std::uint64_t> sCollidableEntitiesByChannel;

	private:
		std::float_t mRadius;
		bool mEnabled;
		std::string mCollisionChannel;

		GameObject *mOwner;
		const Vector<Entity*> *mCollidableEntities;

		bool IsColliding(const GameObject& other) const;
		void ResetAttributePointers();
	};

	CONCRETE_ACTION_FACTORY(CircleColliderComponent);
}

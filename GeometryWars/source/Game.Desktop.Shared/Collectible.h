#pragma once
#include "../../source/Library.Desktop/GameObject.h"

namespace Library
{
	class Collectible : public GameObject
	{
		RTTI_DECLARATIONS(Collectible, GameObject);

	public:

		Collectible();
		virtual ~Collectible() = default;

		Collectible(const Collectible& rhs);

		void SetIsCollected(bool isCollected);

		virtual Scope* Clone(const Scope& rhs) const override;
		virtual void BeginPlay(WorldState& worldState) override;
		virtual void Update(WorldState& worldState) override;
		virtual void OnDestroy(WorldState& worldState) override;
		virtual void OnOverlapBegin(const GameObject& other, const std::string& channel, WorldState& worldState) override;

		static const std::string ATTRIBUTE_ISCOLLECTED;
		static const std::string ATTRIBUTE_CHANNEL;
		static const std::string ATTRIBUTE_MOVE_CHANNEL;

	protected:

		bool mIsCollected;
		std::string mCollisionChannel;
		std::string mMoveChannel;

	private:

		void ResetAttributePointers();
	};

	CONCRETE_ENTITY_FACTORY(Collectible);
}


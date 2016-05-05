#pragma once
#include "Enemy.h"

namespace Library
{
	class EnemySquare : public Enemy
	{
		RTTI_DECLARATIONS(EnemySquare, Enemy);

	public:

		EnemySquare();
		virtual ~EnemySquare() = default;

		EnemySquare(const EnemySquare& rhs);

		virtual Scope* Clone(const Scope& rhs) const override;
		virtual void BeginPlay(WorldState& worldState) override;
		virtual void Update(WorldState& worldState) override;
		virtual void OnOverlapBegin(const GameObject& other, const std::string& channel, WorldState& worldState) override;

		static const std::string ATTRIBUTE_DODGE_CHANNEL;
		static const std::string ATTRIBUTE_DODGE_ANGLE;
		static const std::string ATTRIBUTE_DODGING;

	protected:

		std::string mDodgeChannel;
		bool mIsDodging;
		std::float_t mDodgeAngle;

	private:

		void ResetAttributePointers();
	};

	CONCRETE_ENTITY_FACTORY(EnemySquare);
}
#pragma once
#include "Action.h"

namespace Library
{
	class ActionDestroyEntity : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyEntity, Action);

	public:
		ActionDestroyEntity();
		virtual ~ActionDestroyEntity() = default;

		virtual void Update(WorldState& worldState) override;

		static const std::string ATTRIBUTE_ENTITY_INSTANCE_NAME;

	private:
		std::string mEntityToDestroy;
	};

	CONCRETE_ACTION_FACTORY(ActionDestroyEntity);
}


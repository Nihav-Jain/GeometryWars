#pragma once
#include "Action.h"

namespace Library
{
	/**
	 *	Destriys an entity by reference
	 *	@inherits ActionList
	 */
	class ActionDestroyEntity : public ActionList
	{
		RTTI_DECLARATIONS(ActionDestroyEntity, ActionList);

	public:
		ActionDestroyEntity();
		virtual ~ActionDestroyEntity();

		ActionDestroyEntity(const ActionDestroyEntity& rhs);
		ActionDestroyEntity(ActionDestroyEntity&& rhs);

		ActionDestroyEntity& operator=(const ActionDestroyEntity& rhs);
		ActionDestroyEntity& operator=(ActionDestroyEntity&& rhs);

		virtual void Update(WorldState& worldState) override;

		virtual Scope* Clone(const Scope& rhs) const override;

		static const std::string ATTRIBUTE_ENTITY_REFERENCE;

	private:
		Datum mEntityToDestroy;
	};

	CONCRETE_ACTION_FACTORY(ActionDestroyEntity);
}


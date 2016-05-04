#pragma once
#include "Action.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	 *	Creates an Entity from a constructed saved state
	 *	@inherits Action
	 */
	class ActionCreateEntityFromFile : public Action
	{
		RTTI_DECLARATIONS(ActionCreateEntityFromFile, Action);

	public:
		ActionCreateEntityFromFile();
		virtual ~ActionCreateEntityFromFile();

		ActionCreateEntityFromFile(const ActionCreateEntityFromFile& rhs);
		ActionCreateEntityFromFile(ActionCreateEntityFromFile&& rhs);

		ActionCreateEntityFromFile& operator=(const ActionCreateEntityFromFile& rhs);
		ActionCreateEntityFromFile& operator=(ActionCreateEntityFromFile&& rhs);

		virtual void Update(WorldState& worldState) override;

		virtual Scope* Clone(const Scope& rhs) const override;

		static const std::string ATTRIBUTE_FILE_PATH;
		static const std::string ATTRIBUTE_ENTITY_CLASS_NAME;
		static const std::string ATTRIBUTE_ENTITY_INSTANCE_NAME;
		static const std::string ATTRIBUTE_NEW_ENTITY;

	private:
		Datum* mNewEntity;
	};

	CONCRETE_ACTION_FACTORY(ActionCreateEntityFromFile);
}


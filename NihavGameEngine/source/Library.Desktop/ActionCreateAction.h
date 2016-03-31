#pragma once
#include "Action.h"

namespace Library
{
	/**
	 *	Creates an Action of the specified class at runtime and appends it to its parent
	 *	Has 3 prescribed attribute - name (STRING), prototypeClass (STRING), prototypeName (STRING)
	 *	@grammar <action class="ActionCreateAction" name="{STRING}" prototypeClass="{STRING}" prototypeName="{STRING}" />
	 *	@requires XmlParseHelperActionCreate to being created via XML
	 *	@inherits Action
	 */
	class ActionCreateAction : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action);
	public:
		
		/**
		 *	Constructor - initializes member variables and declares prescribed attributes
		 */
		ActionCreateAction();

		/**
		 *	Default destructor
		 */
		virtual ~ActionCreateAction() = default;

		/**
		 *	disallow copy construtor
		 */
		ActionCreateAction(const ActionCreateAction& rhs) = delete;
		
		/**
		 *	disallow copy assignment operator
		 */
		ActionCreateAction& operator=(const ActionCreateAction& rhs) = delete;

		/**
		 *	Calls CreateAction on its parent
		 *	@param reference to the WorldState
		 */
		virtual void Update(WorldState& worldState) override;

		static const std::string ATTRIBUTE_PROTOTYPE_CLASS;
		static const std::string ATTRIBUTE_PROTOTYPE_NAME;

	protected:
		virtual void DefinePrescribedAttributes() override;
	};
	
	CONCRETE_ACTION_FACTORY(ActionCreateAction);
}


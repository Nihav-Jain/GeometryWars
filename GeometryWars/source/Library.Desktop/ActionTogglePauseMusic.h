#pragma once


namespace Library
{

	class ActionTogglePauseMusic : public ActionList
	{
		RTTI_DECLARATIONS(ActionTogglePauseMusic, ActionList)


	public:
		ActionTogglePauseMusic();

		virtual ~ActionTogglePauseMusic() = default;

		ActionTogglePauseMusic& operator=(const ActionTogglePauseMusic& rhs) = delete;


		/**
		*	Calls the update method on all of its child Actions, called by the parent Action / Entity / Sector / World's Update method
		*	@param reference to the WorldState
		*/
		virtual void Update(WorldState& worldState) override;

		Scope * Clone(const Scope & rhs) const;

		static const std::string ATTRIBUTE_NAME;
	};

	CONCRETE_ACTION_FACTORY(ActionTogglePauseMusic);

}
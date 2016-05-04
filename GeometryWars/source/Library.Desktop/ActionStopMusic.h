#pragma once


namespace Library
{

	class ActionStopMusic : public ActionList
	{
		RTTI_DECLARATIONS(ActionStopMusic, ActionList)


	public:
		ActionStopMusic();
		virtual ~ActionStopMusic() = default;

		ActionStopMusic& operator=(const ActionStopMusic& rhs) = delete;


		Scope * Clone(const Scope & rhs) const;

		/**
		*	Calls the update method on all of its child Actions, called by the parent Action / Entity / Sector / World's Update method
		*	@param reference to the WorldState
		*/
		virtual void Update(WorldState& worldState) override;

		static const std::string ATTRIBUTE_NAME;
	};

	CONCRETE_ACTION_FACTORY(ActionStopMusic);

}
#pragma once

namespace Library
{

	class ActionLoadMusic : public ActionList
	{
		RTTI_DECLARATIONS(ActionLoadMusic, ActionList);
	
	
	public:
		ActionLoadMusic();
		virtual ~ActionLoadMusic() = default;

		ActionLoadMusic(const ActionLoadMusic& rhs) = delete;
		ActionLoadMusic& operator=(const ActionLoadMusic& rhs) = delete;


		/**
		*	Calls the update method on all of its child Actions, called by the parent Action / Entity / Sector / World's Update method
		*	@param reference to the WorldState
		*/
		virtual void Update(WorldState& worldState) override;

		static const std::string ATTRIBUTE_NAME;
		
	private:

	};

	CONCRETE_ACTION_FACTORY(ActionLoadMusic);
}

#pragma once
#include "Action.h"
#include "Entity.h"
#include <Windows.h>
#include <Xinput.h>

namespace Library
{
	// Generic Input Handler
	class InputHandler : public Action
	{
		RTTI_DECLARATIONS(InputHandler, Action)
	public:
		InputHandler();
		virtual ~InputHandler() = default;
		virtual void Initialize() = 0;
	};



	// KeyBoard Input Event Handler
	class KeyBoardHandler final : public InputHandler
	{
		RTTI_DECLARATIONS(KeyBoardHandler, InputHandler)
	public:
		void Initialize() override;
		void Update(WorldState& state) override;
	};
	CONCRETE_ACTION_FACTORY(KeyBoardHandler)




	// XBox Controller Input Event Handler
	class XBoxControllerHandler final : public InputHandler
	{
		RTTI_DECLARATIONS(XBoxControllerHandler, InputHandler)
	private:
		// GamePad States
		XINPUT_STATE mState[4];					// GamePad State for each player (max. 4)
		bool bIsPlayersConnected[4];			// Checks which players are connected

		void UpdateState();						// Get the latest State of all players
		bool IsConnected(std::int32_t player);	// Check for player connection

		//

		// Thumbstick functions
		// - Return true if stick is inside deadzone, false if outside
		bool LStick_InDeadzone(std::int32_t player);
		bool RStick_InDeadzone(std::int32_t player);

		float LeftStick_X(std::int32_t player);  // Return X axis of left stick
		float LeftStick_Y(std::int32_t player);  // Return Y axis of left stick
		float RightStick_X(std::int32_t player); // Return X axis of right stick
		float RightStick_Y(std::int32_t player); // Return Y axis of right stick

		// Trigger functions
		float LeftTrigger(std::int32_t player);  // Return value of left trigger
		float RightTrigger(std::int32_t player); // Return value of right trigger

		// Vibrate the gamepad (0.0f cancel, 1.0f max speed)
		void Rumble(std::int32_t player, float a_fLeftMotor = 0.0f, float a_fRightMotor = 0.0f);

	public:
		XBoxControllerHandler();
		~XBoxControllerHandler() override;

		void Initialize() override;
		void Update(WorldState& state) override;
	};
	CONCRETE_ACTION_FACTORY(XBoxControllerHandler)





	// Input Manager
	/*
	class InputManager final : public ActionList
	{
		RTTI_DECLARATIONS(InputManager, Action)

	private:
		//static const std::string INPUT_HANDLER_LIST;

	public:
		InputManager();
		~InputManager();

		//void Update(WorldState& state) override;
	};
	CONCRETE_ACTION_FACTORY(InputManager)
	*/
}


#pragma once
#include "Action.h"
#include "Entity.h"
#include <Windows.h>
#include <Xinput.h>

namespace Library
{
	class EventMessageAttributed;

	// Generic Input Handler
	class InputHandler : public Action
	{
		RTTI_DECLARATIONS(InputHandler, Action)
	protected:
		// IOEventTypes - Represents common events among all input devices
		enum class EIOEventType
		{
			PLAYER_CONNECTED = 0,
			PLAYER_DISCONNECTED,
			NUMBER_OF_IO_EVENT_TYPES
		};
		// Get String Representation of the IOEventTypes
		static std::string GetIOEventType(const EIOEventType& type);
		// Easy Accessor to retrieve the XML-Defined Button Mapping
		Scope& GetButtonMapping();
		// Easy Method Call to Send Button Pressed Events
		void SendButtonEvent(std::string buttonEventName, WorldState& state, EventMessageAttributed & message, Scope* ButtonMapping = nullptr);
		// Easy Method Call to Send Common Input Events
		void SendIOEvent(const EIOEventType & eventType, WorldState& state, EventMessageAttributed & message);
		
	private:
		// The Name of the XML-Defined ButtonMapping Scope
		static const std::string ATTR_BUTTON_MAP;
		// Array used to map enumerated EIOEventType to string
		static const std::string sIOEventTypeToString[static_cast<std::int32_t>(EIOEventType::NUMBER_OF_IO_EVENT_TYPES)];

	public:
		InputHandler();
		virtual ~InputHandler() = default;
	};



	// KeyBoard Input Event Handler
	class KeyBoardHandler final : public InputHandler
	{
		RTTI_DECLARATIONS(KeyBoardHandler, InputHandler)
	public:
		void Update(WorldState& state) override;
	};
	CONCRETE_ACTION_FACTORY(KeyBoardHandler)


	// XBox Controller Input Event Handler
	class XBoxControllerHandler final : public InputHandler
	{
		RTTI_DECLARATIONS(XBoxControllerHandler, InputHandler)
	private:
		static Hashmap<std::string, std::int32_t> XBoxButtonMapping;	// Button Support
		static const std::uint32_t MAX_PLAYERS = 4;						// Maximum Players this handler can support

		// GamePad States
		XINPUT_STATE mState[MAX_PLAYERS];		// GamePad State for each player (max. 4)
		bool bIsPlayersConnected[MAX_PLAYERS];	// Checks which players are connected

	private:
		// NOTE:	Most of the Methods I got from a tutorial site, and were used for
		//			Testing if XInput worked and to guide the structure of the Update call
		//			All of the work is actually done in the Update(WorldState&) Call
		//			Future iterations of this Handler will clean up and make them useful
		//			For now, they are not being used...
		bool IsConnected(std::int32_t player);	// Check for player connection

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

		// Button functions
		// - 'Pressed' - Return true if pressed, false if not
		bool GetButtonPressed(std::int32_t player, std::string button);

	public:
		XBoxControllerHandler();
		~XBoxControllerHandler() override;

		void Update(WorldState& state) override;
	};
	CONCRETE_ACTION_FACTORY(XBoxControllerHandler)



	//	Input Manager
	/*	NOTE: Debating how viable it is to have an InputManager...
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


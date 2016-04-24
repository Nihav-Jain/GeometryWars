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
		// To help calculate and set input state durations
		static const std::chrono::milliseconds zero_ms;
		static const std::chrono::milliseconds negative_ms;

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
		//void SendButtonEvent(std::string buttonEventName, WorldState& state, EventMessageAttributed & message, Scope* ButtonMapping = nullptr);
		void SendButtonEvent(std::string buttonEventName, WorldState& state, EventMessageAttributed & message, const Datum& eventNames);
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


	struct Button
	{
		std::chrono::milliseconds	Duration;	// If negative, button not pressed (released), if positive, duration of press in milliseconds, if zero, just pressed
	};

	struct KeyState
	{
		std::int32_t	Key;
		Button			Info;
	};

	// KeyBoard Input Event Handler
	class KeyBoardHandler final : public InputHandler
	{
		RTTI_DECLARATIONS(KeyBoardHandler, InputHandler)
	public:
		void Update(WorldState& state) override;
	};
	CONCRETE_ACTION_FACTORY(KeyBoardHandler)


	struct Trigger
	{
		std::chrono::milliseconds	Duration;	// If negative, trigger not pressed (released), if positive, duration of press in milliseconds
		std::int32_t				Magnitude;	// 1-Dimensional Data
		std::float_t	UnitMagnitude()	const { return Magnitude / 255.0f; }
	};

	struct AnalogStick
	{
		std::chrono::milliseconds	Duration;	// If negative, analog is still, if positive, duration of activity in milliseconds
		std::int32_t	MagnitudeX, MagnitudeY;
		std::float_t	UnitMagnitudeX() const { return (static_cast<float>(MagnitudeX) / 32768.0f); }
		std::float_t	UnitMagnitudeY() const { return (static_cast<float>(MagnitudeY) / 32768.0f); }
		glm::vec2		UnitVector2D() const { return glm::normalize(glm::vec2(MagnitudeX, MagnitudeY)); }
		glm::vec4		UnitVector4D() const { return glm::normalize(glm::vec4(MagnitudeX, MagnitudeY, 0, 0)); }
	};

	// XBox Controller Information
	struct XBoxControllerState
	{
		// Buttons
		Button A, B, X, Y;
		Button DPad_Up, DPad_Down, DPad_Left, DPad_Right;
		Button Left_Shoulder, Right_Shoulder;
		Button Left_Thumbstick, Right_Thumbstick;
		Button Start, Back;
		// Trigger
		Trigger LeftTrigger, RightTrigger;
		// Analog
		AnalogStick LeftStick, RightStick;
	};

	// XBox Controller Input Event Handler
	class XBoxControllerHandler final : public InputHandler
	{
		RTTI_DECLARATIONS(XBoxControllerHandler, InputHandler)
	private:
		static Hashmap<std::string, std::int32_t> XBoxButtonMapping;	// Button Support
		static const std::uint32_t MAX_PLAYERS = 4;						// Maximum Players this handler can support

		// GamePad States
		//XINPUT_STATE mState[MAX_PLAYERS];				// GamePad State for each player (max. 4)
		bool bIsPlayersConnected[MAX_PLAYERS];			// Checks which players are connected
		XBoxControllerState mPlayerState[MAX_PLAYERS];
		WORD mButtonState[MAX_PLAYERS];					// Boolean Array of the Button states

	private:
		// Vibrate the gamepad (0.0f cancel, 1.0f max speed)
		void Rumble(std::int32_t player, float a_fLeftMotor = 0.0f, float a_fRightMotor = 0.0f);

		void ChangeButtonState(Button& playerButton, const std::chrono::milliseconds& deltaTime, bool IsPressed);
		void ChangeTriggerState(Trigger& playerTrigger, const std::chrono::milliseconds& deltaTime, const SHORT& magnitude, const SHORT& threshold);
		void ChangeAnalogState(AnalogStick& playerAnalog, const std::chrono::milliseconds& deltaTime, const SHORT& magnitudeX, const SHORT& magnitudeY, const SHORT& threshold);
		void UpdatePlayerState(std::uint32_t player, XINPUT_STATE& currentPlayerState, std::chrono::milliseconds deltaTime);
	public:
		XBoxControllerHandler();
		~XBoxControllerHandler() override;

		// Check for player connection
		bool IsConnected(std::uint32_t player);	
		// Get Player State
		const XBoxControllerState& GetPlayerState(std::uint32_t player);
		// Update XBox Controller state
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


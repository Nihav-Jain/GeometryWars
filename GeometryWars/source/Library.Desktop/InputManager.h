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
		InputHandler(const InputHandler& other) = default;
		InputHandler(InputHandler&& other) = default;
		InputHandler& operator=(const InputHandler& rhs) = default;
		InputHandler& operator=(InputHandler&& rhs) = default;
		virtual ~InputHandler() = default;
	};


	class Button : public Attributed
	{
		RTTI_DECLARATIONS(Button, Attributed)
	private:
		std::int32_t	mDuration;	// If negative, button not pressed (released), if positive, duration of press in milliseconds, if zero, just pressed

	public:
		bool			IsKeyDown;
		bool			IsOnPress;
		bool			IsOnRelease;
	public:
		Button();
		Button(const Button& other) = default;
		Button(Button&& other) = default;
		Button& operator=(const Button& rhs) = default;
		Button& operator=(Button&& rhs) = default;
		void UpdateState(const std::chrono::milliseconds& deltaTime, bool IsPressed);
	};

	class Trigger : public Attributed
	{
		RTTI_DECLARATIONS(Trigger, Attributed)
	private:
		std::int32_t	mDuration;	// If negative, trigger not pressed (released), if positive, duration of press in milliseconds
		std::int32_t	mRaw;	// 1-Dimensional Data
	public:
		std::float_t	Magnitude;
		bool			IsKeyDown;
		bool			IsOnPress;
		bool			IsOnRelease;

	public:
		Trigger();
		Trigger(const Trigger& other) = default;
		Trigger(Trigger&& other) = default;
		Trigger& operator=(const Trigger& rhs) = default;
		Trigger& operator=(Trigger&& rhs) = default;
		void UpdateState(const std::chrono::milliseconds&, const std::int32_t& newMagnitude, const std::int32_t& threshold);
	};

	class AnalogStick : public Attributed
	{
		RTTI_DECLARATIONS(AnalogStick, Attributed)
	private:
		std::int32_t	mDuration;	// If negative, analog is still, if positive, duration of activity in milliseconds
		std::int32_t	mRawX, mRawY;

	public:
		glm::vec4		MagnitudeVector;
		glm::vec4		RotationVector;
		glm::vec4		Direction;
		std::float_t	MagnitudeX;
		std::float_t	MagnitudeY;
		std::float_t	Magnitude;
		std::float_t	Rotation;
		bool			IsKeyDown;
		bool			IsOnPress;
		bool			IsOnRelease;

	public:
		AnalogStick();
		AnalogStick(const AnalogStick& other) = default;
		AnalogStick(AnalogStick&& other) = default;
		AnalogStick& operator=(const AnalogStick& rhs) = default;
		AnalogStick& operator=(AnalogStick&& rhs) = default;
		void UpdateState(const std::chrono::milliseconds& deltaTime, const std::int32_t& newMagnitudeX, const std::int32_t& newMagnitudeY, const std::int32_t& threshold);
	};

	// XBox Controller Information
	struct XBoxControllerState : public Attributed
	{
		RTTI_DECLARATIONS(XBoxControllerState, Attributed)
	public:
		// Buttons
		Button A, B, X, Y;
		Button DPad_Up, DPad_Down, DPad_Left, DPad_Right;
		Button LeftShoulder, RightShoulder;
		Button LeftThumbstick, RightThumbstick;
		Button Start, Back;
		// Trigger
		Trigger LeftTrigger, RightTrigger;
		// Analog
		AnalogStick LeftStick, RightStick;

	public:
		XBoxControllerState();
		XBoxControllerState(const XBoxControllerState& other) = default;
		XBoxControllerState(XBoxControllerState&& other) = default;
		XBoxControllerState& operator=(const XBoxControllerState& rhs) = default;
		XBoxControllerState& operator=(XBoxControllerState&& rhs) = default;
		void UpdateState(XINPUT_STATE & currentPlayerState, std::chrono::milliseconds deltaTime);
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
		bool bIsPlayerConnected[MAX_PLAYERS];			// Checks which players are connected
		XBoxControllerState mPlayerState[MAX_PLAYERS];			// XBox Controller state for each player
		WORD mButtonState[MAX_PLAYERS];					// Boolean Array of the Button states

	private:
		// Vibrate the gamepad (0.0f cancel, 1.0f max speed)
		void Rumble(std::int32_t player, float a_fLeftMotor = 0.0f, float a_fRightMotor = 0.0f);

	public:
		XBoxControllerHandler();
		XBoxControllerHandler(const XBoxControllerHandler& other) = default;
		XBoxControllerHandler(XBoxControllerHandler&& other) = default;
		XBoxControllerHandler& operator=(const XBoxControllerHandler& rhs) = default;
		XBoxControllerHandler& operator=(XBoxControllerHandler&& rhs) = default;
		~XBoxControllerHandler() override;

		// Check for player connection
		bool IsConnected(std::uint32_t player);	
		// Get Player State
		const XBoxControllerState& GetPlayerState(std::uint32_t player);
		// Update XBox Controller state
		void Update(WorldState& state) override;
	};
	CONCRETE_ACTION_FACTORY(XBoxControllerHandler)
}


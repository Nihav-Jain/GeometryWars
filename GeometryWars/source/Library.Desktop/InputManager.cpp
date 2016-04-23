#include "pch.h"
#include "InputManager.h"
#include "Event.h"
#include "Datum.h"
#include "EventMessageAttributed.h"
#include <winerror.h>

#pragma once
// Link the 'Xinput' library - Important!
//#pragma comment(lib, "Xinput.lib")

namespace Library
{
	/*************************
	**	Input Handler CPP	**
	**************************/
	RTTI_DEFINITIONS(InputHandler)

	const std::string InputHandler::ATTR_BUTTON_MAP = "ButtonMapping";
	const std::string InputHandler::sIOEventTypeToString[] = { "PlayerConnected", "PlayerDisconnected" };

	std::string InputHandler::GetIOEventType(const EIOEventType& type) 
	{
		return sIOEventTypeToString[static_cast<std::int32_t>(type)];
	}


	InputHandler::InputHandler()
	{
		// XML-Define Button Mapping which maps the Event's Subtype with the Buttons
		AddNestedScope(ATTR_BUTTON_MAP);
	}

	Scope& InputHandler::GetButtonMapping()
	{
		Datum* buttonMappingDatum = Find(ATTR_BUTTON_MAP);
		assert(buttonMappingDatum != nullptr && buttonMappingDatum->Type() == Datum::DatumType::TABLE);
		return buttonMappingDatum->Get<Scope>();
	}

	void InputHandler::SendButtonEvent(std::string buttonEventName, WorldState& state, EventMessageAttributed & message, Scope* buttonMap)
	{
		// Test if ButtonMapping is passed in (for the purpose of speed)
		if (buttonMap == nullptr)
		{
			buttonMap = &(GetButtonMapping());
		}

		// Set Subtype based on Button Mapping
		Datum& subTypeForEvent = (*buttonMap)[buttonEventName];
		// Set Message's WorldState
		message.SetWorldState(state);

		// Send Event for Every Event Name mapped to this button
		for (std::uint32_t i = 0; i < subTypeForEvent.Size(); ++i)
		{
			// Set SubType based on Event Names in ButtonMapping scope
			message.SetSubtype(subTypeForEvent.Get<std::string>(i));
			// Send Event
			std::shared_ptr<Event<EventMessageAttributed>> buttonChangeEvent = std::make_shared<Event<EventMessageAttributed>>(message);
			state.world->GetEventQueue().Send(buttonChangeEvent);
		}
	}

	void InputHandler::SendIOEvent(const EIOEventType & eventType, WorldState& state, EventMessageAttributed & message)
	{
		// Set Subtype based on IOEventType
		message.SetSubtype(GetIOEventType(eventType));
		// Set Message's WorldState
		message.SetWorldState(state);
		// Send Event
		std::shared_ptr<Event<EventMessageAttributed>> buttonChangeEvent = std::make_shared<Event<EventMessageAttributed>>(message);
		state.world->GetEventQueue().Send(buttonChangeEvent);
	}

	/*************************
	**	KeyboardHandler CPP	**
	**************************/
	RTTI_DEFINITIONS(KeyBoardHandler)

	void KeyBoardHandler::Update(WorldState& state)
	{
		(state);
	}

	/*************************
	**	XBox Controller CPP	**
	**************************/
	RTTI_DEFINITIONS(XBoxControllerHandler)

	Hashmap<std::string, std::int32_t> XBoxControllerHandler::XBoxButtonMapping({
		std::pair<std::string, std::int32_t>("A",				XINPUT_GAMEPAD_A),
		std::pair<std::string, std::int32_t>("B",				XINPUT_GAMEPAD_B),
		std::pair<std::string, std::int32_t>("X",				XINPUT_GAMEPAD_X),
		std::pair<std::string, std::int32_t>("Y",				XINPUT_GAMEPAD_Y),
		std::pair<std::string, std::int32_t>("DPAD_UP",			XINPUT_GAMEPAD_DPAD_UP),
		std::pair<std::string, std::int32_t>("DPAD_DOWN",		XINPUT_GAMEPAD_DPAD_DOWN),
		std::pair<std::string, std::int32_t>("DPAD_LEFT",		XINPUT_GAMEPAD_DPAD_LEFT),
		std::pair<std::string, std::int32_t>("DPAD_RIGHT",		XINPUT_GAMEPAD_DPAD_RIGHT),
		std::pair<std::string, std::int32_t>("LEFT_SHOULDER",	XINPUT_GAMEPAD_LEFT_SHOULDER),
		std::pair<std::string, std::int32_t>("RIGHT_SHOULDER",	XINPUT_GAMEPAD_RIGHT_SHOULDER),
		std::pair<std::string, std::int32_t>("LEFT_THUMB",		XINPUT_GAMEPAD_LEFT_THUMB),
		std::pair<std::string, std::int32_t>("RIGHT_THUMB",		XINPUT_GAMEPAD_RIGHT_THUMB),
		std::pair<std::string, std::int32_t>("START",			XINPUT_GAMEPAD_START),
		std::pair<std::string, std::int32_t>("BACK",			XINPUT_GAMEPAD_BACK)
	});
	
	XBoxControllerHandler::XBoxControllerHandler()
	{
		// Expose bIsPlayersConnected to the XML
		AddExternalAttribute("IsPlayersConnected", 4, bIsPlayersConnected);
		// NOTE: Exposing the State of the Button will be for future iterations
	}

	XBoxControllerHandler::~XBoxControllerHandler()	{}

	bool XBoxControllerHandler::IsConnected(std::int32_t player)
	{
		return bIsPlayersConnected[player];
	}

	bool XBoxControllerHandler::LStick_InDeadzone(std::int32_t player)
	{
		// Obtain the X & Y axes of the stick
       short sX = mState[player].Gamepad.sThumbLX;
	   short sY = mState[player].Gamepad.sThumbLY;

	   // X axis is outside of deadzone
	   if (sX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		   sX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		   return false;

	   // Y axis is outside of deadzone
	   if (sY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		   sY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		   return false;

	   // One (or both axes) axis is inside of deadzone
	   return true;
	}

	bool XBoxControllerHandler::RStick_InDeadzone(std::int32_t player)
	{
		// Obtain the X & Y axes of the stick
		short sX = mState[player].Gamepad.sThumbRX;
		short sY = mState[player].Gamepad.sThumbRY;

		// X axis is outside of deadzone
		if (sX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
			sX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return false;

		// Y axis is outside of deadzone
		if (sY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
			sY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return false;

		// One (or both axes) axis is inside of deadzone
		return true;
	}

	float XBoxControllerHandler::LeftStick_X(std::int32_t player)
	{
		// Obtain X axis of left stick
		short sX = mState[player].Gamepad.sThumbLX;

		// Return axis value, converted to a float
		return (static_cast<float>(sX) / 32768.0f);
	}

	float XBoxControllerHandler::LeftStick_Y(std::int32_t player)
	{
		// Obtain Y axis of left stick
		short sY = mState[player].Gamepad.sThumbLY;

		// Return axis value, converted to a float
		return (static_cast<float>(sY) / 32768.0f);
	}

	float XBoxControllerHandler::RightStick_X(std::int32_t player)
	{
		// Obtain Y axis of left stick
		short sY = mState[player].Gamepad.sThumbRX;

		// Return axis value, converted to a float
		return (static_cast<float>(sY) / 32768.0f);
	}

	float XBoxControllerHandler::RightStick_Y(std::int32_t player)
	{
		// Obtain Y axis of left stick
		short sY = mState[player].Gamepad.sThumbRY;

		// Return axis value, converted to a float
		return (static_cast<float>(sY) / 32768.0f);
	}

	float XBoxControllerHandler::LeftTrigger(std::int32_t player)
	{
		// Obtain value of left trigger
        BYTE Trigger = mState[player].Gamepad.bLeftTrigger;

		if (Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			return Trigger / 255.0f;

		return 0.0f; // Trigger was not pressed
	}

	float XBoxControllerHandler::RightTrigger(std::int32_t player)
	{
		// Obtain value of right trigger
		BYTE Trigger = mState[player].Gamepad.bRightTrigger;

		if (Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			return Trigger / 255.0f;

		return 0.0f; // Trigger was not pressed
	}

	void XBoxControllerHandler::Rumble(std::int32_t player, float a_fLeftMotor, float a_fRightMotor)
	{
		// Vibration state
		XINPUT_VIBRATION VibrationState;

		// Zero memory
		RtlSecureZeroMemory(&VibrationState, sizeof(XINPUT_VIBRATION));

		// Calculate vibration values
		int iLeftMotor = int(a_fLeftMotor * 65535.0f);
		int iRightMotor = int(a_fRightMotor * 65535.0f);

		// Set vibration values
		VibrationState.wLeftMotorSpeed = (WORD)iLeftMotor;
		VibrationState.wRightMotorSpeed = (WORD)iRightMotor;

		// Set the vibration state for that player
		XInputSetState(player, &VibrationState);
	}
	
	bool XBoxControllerHandler::GetButtonPressed(std::int32_t player, std::string button)
	{
		// Compare Gamepad's button bits to see if the button of choice got pressed
		return ((mState[player].Gamepad.wButtons & XBoxButtonMapping[button]) != 0);
	}

	void XBoxControllerHandler::Update(WorldState& state)
	{
		// Obtain Current XBox Controller State for All Players
		XINPUT_STATE currentState[4];
		bool currentPlayersConnected[4];
		for (int i = 0; i < 4; ++i)
		{
			// Zero memory
			RtlSecureZeroMemory(&currentState[i], sizeof(XINPUT_STATE));

			// Get the state of the gamepad
			DWORD Result = XInputGetState(i, &currentState[i]);

			// Update Connections
			currentPlayersConnected[i] = (Result == ERROR_SUCCESS);
		}

		// Test For Events (Button Changes, Player Connection Changes, etc.)
		Scope& ButtonMapping = GetButtonMapping();
		for (int player = 0; player < 4; ++player)
		{
			// State of Player connection changed
			if (bIsPlayersConnected[player] != currentPlayersConnected[player])
			{
				// Player's Connection State Changed, Send Event
				EventMessageAttributed message;
				message.AppendAuxiliaryAttribute("PlayerNumber") = player;
					
				// Determine whether event is connection gain or lost
				EIOEventType type = (currentPlayersConnected[player]) ? EIOEventType::PLAYER_CONNECTED : EIOEventType::PLAYER_DISCONNECTED;

				// Send Player Connection Change Event
				SendIOEvent(type, state, message);
			}

			// Terminate Event Checks for player if player is not connected
			if (!currentPlayersConnected[player])
			{
				continue;
			}

			// Detect Changes for the Button states between Frames
			// Note:	wButton is a WORD that represents a boolean array for button states
			//			XOR between previous and current state will result in marking only the bits of the buttons that changed
			WORD buttonStateChanges = (mState[player].Gamepad.wButtons ^ currentState[player].Gamepad.wButtons);

			//  Quickly Check for Button Changes (0 if no button has changed)
			if (buttonStateChanges)
			{
				// Loop through all Buttons and their button states
				for (auto& buttonPair : XBoxButtonMapping)
				{
					// Check if Button has changed since last frame
					if (buttonStateChanges & buttonPair.second)
					{
						bool isButtonPressed = ((currentState[player].Gamepad.wButtons & buttonPair.second) != 0);

						// Button State Changed, Enqueue Button changed event
						EventMessageAttributed message;
						message.AppendAuxiliaryAttribute("PlayerNumber") = player;	// Store Player Number
						message.AppendAuxiliaryAttribute("IsButtonPressed") = isButtonPressed;

						SendButtonEvent(buttonPair.first, state, message, &ButtonMapping);
					}
				}
			}
		}

		// Update All Current Results
		for (int player = 0; player < 4; ++player)
		{
			mState[player] = currentState[player];
			bIsPlayersConnected[player] = currentPlayersConnected[player];
		}
	}


	/*************************
	**	Input Manager CPP	**
	**************************/
	/*
	RTTI_DEFINITIONS(InputManager)
	//const std::string InputManager::INPUT_HANDLER_LIST = "InputHandlers";

	InputManager::InputManager()
	{
		// Add Scope Name for all InputHandlers created
		//AddNestedScope(INPUT_HANDLER_LIST);
	}

	InputManager::~InputManager()
	{
	}
	void InputManager::Update(WorldState & state)
	{
		// Get Delta Time
		std::chrono::milliseconds deltaTime = state.mGameTime->ElapsedGameTime();

		// Look through Input Handler List and Update All Handlers
		Datum& inputHandlerList = Append(INPUT_HANDLER_LIST);
		for (std::uint32_t i = 0; i < inputHandlerList.Size(); ++i)
		{
			Scope& scope = inputHandlerList.Get<Scope>(i);
			if (InputHandler* handler = scope.As<InputHandler>())
			{
				handler->Update(deltaTime);
			}
			else
			{
				throw std::exception("Invalid Input Handler assigned to the Input Handler List.");
			}
		}
	}*/
}

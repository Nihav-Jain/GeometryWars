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
	RTTI_DEFINITIONS(InputHandler, Action)

	const std::string				InputHandler::ATTR_BUTTON_MAP			= "ButtonMapping";
	const std::string				InputHandler::sIOEventTypeToString[]	= { "PlayerConnected", "PlayerDisconnected" };

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

	//void InputHandler::SendButtonEvent(std::string buttonEventName, WorldState& state, EventMessageAttributed & message, Scope* buttonMap)
	void InputHandler::SendButtonEvent(std::string buttonEventName, WorldState& state, EventMessageAttributed & message, const Datum& eventNames)
	{
		// Set Message's WorldState
		message.SetWorldState(state);

		// Send Event for Every Event Name mapped to this button
		for (std::uint32_t i = 0; i < eventNames.Size(); ++i)
		{
			// Set SubType based on Event Names in ButtonMapping scope
			message.SetSubtype(eventNames.Get<std::string>(i));
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
	**	XBox Controller CPP	**
	**************************/
	RTTI_DEFINITIONS(XBoxControllerHandler, InputHandler)
	RTTI_DEFINITIONS(Button, Attributed)
	RTTI_DEFINITIONS(Trigger, Attributed)
	RTTI_DEFINITIONS(AnalogStick, Attributed)
	RTTI_DEFINITIONS(XBoxControllerState, Attributed)

#define X_ADDDATA(Data)			AddExternalAttribute(#Data, 1, &Data)
	Button::Button()
	{
		X_ADDDATA(IsKeyDown);
		X_ADDDATA(IsOnPress);
		X_ADDDATA(IsOnRelease);
	}
	Trigger::Trigger()
	{
		X_ADDDATA(Magnitude);
		X_ADDDATA(IsKeyDown);
		X_ADDDATA(IsOnPress);
		X_ADDDATA(IsOnRelease);
	}
	AnalogStick::AnalogStick()
	{
		X_ADDDATA(MagnitudeVector);
		X_ADDDATA(RotationVector);
		X_ADDDATA(Direction);
		X_ADDDATA(MagnitudeX);
		X_ADDDATA(MagnitudeY);
		X_ADDDATA(Magnitude);
		X_ADDDATA(Rotation);
		X_ADDDATA(IsKeyDown);
		X_ADDDATA(IsOnPress);
		X_ADDDATA(IsOnRelease);
	}
#undef X_ADDDATA

#define X_ADDSCOPE(ScopeData)	AddNestedScope(#ScopeData, ScopeData, 1U)
	XBoxControllerState::XBoxControllerState()
	{
		X_ADDSCOPE(A);
		X_ADDSCOPE(B);
		X_ADDSCOPE(Y);
		X_ADDSCOPE(X);
		X_ADDSCOPE(DPad_Up);
		X_ADDSCOPE(DPad_Down);
		X_ADDSCOPE(DPad_Left);
		X_ADDSCOPE(DPad_Right);
		X_ADDSCOPE(LeftShoulder);
		X_ADDSCOPE(RightShoulder);
		X_ADDSCOPE(LeftThumbstick);
		X_ADDSCOPE(RightThumbstick);
		X_ADDSCOPE(Start);
		X_ADDSCOPE(Back);
		X_ADDSCOPE(LeftTrigger);
		X_ADDSCOPE(RightTrigger);
		X_ADDSCOPE(LeftStick);
		X_ADDSCOPE(RightStick);
	}
#undef X_ADDSCOPE
	
	void Button::UpdateState(const std::chrono::milliseconds & deltaTime, bool IsPressed)
	{
		bool PrevKeyDownState = IsKeyDown;
		if (IsPressed)
		{
			if (mDuration < 0)
			{	// If this is the first frame being pressed, start at zero duration
				mDuration = 0;
			}
			else
			{	// else start accumulating the duration
				mDuration += static_cast<int32_t>(deltaTime.count());
			}
		}
		else
		{	// Negative Duration is equivalent to not being pressed
			mDuration = -1;
		}

		// Update KeyDown States
		IsKeyDown = IsPressed;
		if (IsKeyDown != PrevKeyDownState)
		{
			IsOnPress = IsKeyDown;
			IsOnRelease = !IsKeyDown;
		}
		else
		{
			IsOnPress = false;
			IsOnRelease = false;
		}
	}

	void Trigger::UpdateState(const std::chrono::milliseconds&  deltaTime, const std::int32_t& newMagnitude, const std::int32_t& threshold)
	{
		bool PrevKeyDownState = IsKeyDown;
		if (newMagnitude > threshold)
		{
			if (mDuration < 0)
			{	// If this is the first frame being pressed, start at zero duration
				mDuration = 0;
			}
			else
			{	// else start accumulating the duration
				mDuration += static_cast<int32_t>(deltaTime.count());
			}
			mRaw = newMagnitude;
		}
		else
		{	// Negative Duration is equivalent to not being pressed
			mDuration = -1;
			mRaw = 0;
		}

		// Update KeyDown States
		IsKeyDown = mDuration >= 0;
		if (IsKeyDown != PrevKeyDownState)
		{
			IsOnPress = IsKeyDown;
			IsOnRelease = !IsKeyDown;
		}
		else
		{
			IsOnPress = false;
			IsOnRelease = false;
		}

		// Update All Other Variables
		Magnitude = static_cast<float>(mRaw) / 32768.0f;
	}

	void AnalogStick::UpdateState(const std::chrono::milliseconds&  deltaTime, const std::int32_t& newMagnitudeX, const std::int32_t& newMagnitudeY, const std::int32_t& threshold)
	{
		bool PrevKeyDownState = IsKeyDown;
		// Check Distance rather than X and Y separately
		if (newMagnitudeX * newMagnitudeX + newMagnitudeY * newMagnitudeY >= threshold * threshold)
		{
			mRawX = newMagnitudeX;
			mRawY = newMagnitudeY;

			if (mDuration < 0)
			{	// If this is the first frame being pressed, start at zero duration
				mDuration = 0;
			}
			else
			{	// else start accumulating the duration
				mDuration += static_cast<int32_t>(deltaTime.count());
			}
		}
		else
		{
			mRawX = 0;
			mRawY = 0;

			// Negative Duration is equivalent to not being pressed
			mDuration = -1;
		}

		// Update KeyDown States
		IsKeyDown = mDuration >= 0;
		if (IsKeyDown != PrevKeyDownState)
		{
			IsOnPress = IsKeyDown;
			IsOnRelease = !IsKeyDown;
		}
		else
		{
			IsOnPress = false;
			IsOnRelease = false;
		}

		// Update All Other Variables
		MagnitudeX = static_cast<float>(mRawX) / 32768.0f;
		MagnitudeY = static_cast<float>(mRawY) / 32768.0f;
		MagnitudeVector = glm::vec4(MagnitudeX, MagnitudeY, 0, 0);
		Direction = glm::normalize(MagnitudeVector);
		Magnitude = static_cast<float_t>(MagnitudeVector.length());
		Rotation = atan2(MagnitudeY, MagnitudeX);
		RotationVector = glm::vec4(0, 0, Rotation, 0);
	}

#define X_CHECK(Value)	((Value) != 0)
	void XBoxControllerState::UpdateState(XINPUT_STATE & currentPlayerState, std::chrono::milliseconds deltaTime)
	{
		WORD& currentPlayerButtonState = currentPlayerState.Gamepad.wButtons;
		// Update Button State
		A.UpdateState(deltaTime, X_CHECK(currentPlayerButtonState & XINPUT_GAMEPAD_A));
		B.UpdateState(deltaTime, X_CHECK(currentPlayerButtonState & XINPUT_GAMEPAD_B));
		X.UpdateState(deltaTime, X_CHECK(currentPlayerButtonState & XINPUT_GAMEPAD_X));
		Y.UpdateState(deltaTime, X_CHECK(currentPlayerButtonState & XINPUT_GAMEPAD_Y));
		DPad_Up.UpdateState(deltaTime, X_CHECK(currentPlayerButtonState & XINPUT_GAMEPAD_DPAD_UP));
		DPad_Down.UpdateState(deltaTime, X_CHECK(currentPlayerButtonState & XINPUT_GAMEPAD_DPAD_DOWN));
		DPad_Left.UpdateState(deltaTime, X_CHECK(currentPlayerButtonState & XINPUT_GAMEPAD_DPAD_LEFT));
		DPad_Right.UpdateState(deltaTime, X_CHECK(currentPlayerButtonState & XINPUT_GAMEPAD_DPAD_RIGHT));
		LeftShoulder.UpdateState(deltaTime, X_CHECK(currentPlayerButtonState & XINPUT_GAMEPAD_LEFT_SHOULDER));
		RightShoulder.UpdateState(deltaTime, X_CHECK(currentPlayerButtonState & XINPUT_GAMEPAD_RIGHT_SHOULDER));
		LeftThumbstick.UpdateState(deltaTime, X_CHECK(currentPlayerButtonState & XINPUT_GAMEPAD_LEFT_THUMB));
		RightThumbstick.UpdateState(deltaTime, X_CHECK(currentPlayerButtonState & XINPUT_GAMEPAD_RIGHT_THUMB));
		Start.UpdateState(deltaTime, X_CHECK(currentPlayerButtonState & XINPUT_GAMEPAD_START));
		Back.UpdateState(deltaTime, X_CHECK(currentPlayerButtonState & XINPUT_GAMEPAD_BACK));
		// Update Trigger State
		LeftTrigger.UpdateState(deltaTime, currentPlayerState.Gamepad.bLeftTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
		RightTrigger.UpdateState(deltaTime, currentPlayerState.Gamepad.bRightTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
		// Update Analog State
		LeftStick.UpdateState(deltaTime, currentPlayerState.Gamepad.sThumbLX, currentPlayerState.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		RightStick.UpdateState(deltaTime, currentPlayerState.Gamepad.sThumbRX, currentPlayerState.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	}
#undef X_CHECK

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
		// Expose PlayerConnected to the XML
		AddNestedScope("PlayerOneState",	mPlayerState[0], MAX_PLAYERS);
		AddNestedScope("PlayerTwoState",	mPlayerState[1], MAX_PLAYERS);
		AddNestedScope("PlayerThreeState",	mPlayerState[2], MAX_PLAYERS);
		AddNestedScope("PlayerFourState",	mPlayerState[3], MAX_PLAYERS);
		// Expose bIsPlayersConnected to the XML
		AddExternalAttribute("IsPlayerConnected", MAX_PLAYERS, bIsPlayerConnected);

		// Initialize variables
		for (int player = 0; player < MAX_PLAYERS; ++player)
		{
			bIsPlayerConnected[player] = false;
			mButtonState[player] = 0;
		}
	}

	XBoxControllerHandler::~XBoxControllerHandler()	{}

	bool XBoxControllerHandler::IsConnected(std::uint32_t player)
	{
		if (player >= MAX_PLAYERS)
		{
			throw std::exception("Out of Bounds Exception");
		}
		return bIsPlayerConnected[player];
	}

	const XBoxControllerState & XBoxControllerHandler::GetPlayerState(std::uint32_t player)
	{
		if (player >= MAX_PLAYERS)
		{
			throw std::exception("Out of Bounds Exception");
		}
		return mPlayerState[player];
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

	void XBoxControllerHandler::Update(WorldState& state)
	{
		// Obtain Current XBox Controller State for All Players
		XINPUT_STATE currentState[MAX_PLAYERS];
		bool currentPlayersConnected[MAX_PLAYERS];
		for (int i = 0; i < MAX_PLAYERS; ++i)
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
		for (std::int32_t player = 0; player < MAX_PLAYERS; ++player)
		{
			// State of Player connection changed
			if (bIsPlayerConnected[player] != currentPlayersConnected[player])
			{
				// Player's Connection State Changed, Send Event
				EventMessageAttributed message;
				message.AppendAuxiliaryAttribute("PlayerNumber") = player;
					
				// Determine whether event is connection gain or lost
				EIOEventType type = (currentPlayersConnected[player]) ? EIOEventType::PLAYER_CONNECTED : EIOEventType::PLAYER_DISCONNECTED;

				// Send Player Connection Change Event
				SendIOEvent(type, state, message);
			}

			// Update Current Player Connection Results
			bIsPlayerConnected[player] = currentPlayersConnected[player];

			// Terminate Event Checks for player if player is not connected
			if (!currentPlayersConnected[player])
			{
				continue;
			}

			// Detect Changes for the Button states between Frames
			// Note:	wButton is a WORD that represents a boolean array for button states

			//			XOR between previous and current state will result in marking only the bits of the buttons that changed
			WORD& playersCurrentButtonState = currentState[player].Gamepad.wButtons;
			WORD buttonStateChanges = (mButtonState[player] ^ playersCurrentButtonState);

			//  Quickly Check for Button Changes (0 if no button has changed)
			if (buttonStateChanges)
			{
				// Loop through all Buttons and their button states
				for (std::uint32_t i = 0; i < ButtonMapping.Size(); ++i)
				{
					auto& buttonPair = ButtonMapping.GetPair(i);
					std::string buttonName = buttonPair.first;
					const Datum& eventNames = buttonPair.second;
					std::int32_t buttonMask = XBoxButtonMapping[buttonName];

					// Check if Button has changed since last frame
					if (buttonStateChanges & buttonMask)
					{
						bool isButtonPressed = ((playersCurrentButtonState & buttonMask) != 0);

						// Button State Changed, Enqueue Button changed event
						EventMessageAttributed message;
						message.AppendAuxiliaryAttribute("PlayerNumber") = player;	// Store Player Number
						message.AppendAuxiliaryAttribute("IsButtonPressed") = isButtonPressed;

						SendButtonEvent(buttonName, state, message, eventNames);
					}
				}
			}

			// Update Current Player State Results
			mButtonState[player] = playersCurrentButtonState;
			mPlayerState[player].UpdateState(currentState[player], state.mGameTime->ElapsedGameTime());
		}
	}
}

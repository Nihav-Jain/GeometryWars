#include "pch.h"
#include "InputManager.h"
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

	InputHandler::InputHandler()
	{
		// HashMap (hence Scope) which maps the Event's Subtype with the Buttons
		AddNestedScope("ButtonMapping");
	}

	/*************************
	**	KeyboardHandler CPP	**
	**************************/
	RTTI_DEFINITIONS(KeyBoardHandler)

	void KeyBoardHandler::Initialize()
	{
	}
	void KeyBoardHandler::Update(WorldState& state)
	{
		(state);
	}

	/*************************
	**	XBox Controller CPP	**
	**************************/
	RTTI_DEFINITIONS(XBoxControllerHandler)
	
	XBoxControllerHandler::XBoxControllerHandler()
	{
		// Expose bIsPlayersConnected to the XML
		AddExternalAttribute("IsPlayersConnected", 4, bIsPlayersConnected);
	}

	XBoxControllerHandler::~XBoxControllerHandler()
	{
	}

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

	void XBoxControllerHandler::UpdateState()
	{
		// Get the State of ALL Players (if any)
		bool isConnected = false;
		for (int i = 0; i < 4; ++i)
		{
			// Zero memory
			RtlSecureZeroMemory(&mState[i], sizeof(XINPUT_STATE));

			// Get the state of the gamepad
			DWORD Result = XInputGetState(i, &mState[i]);

			// Update Connections
			bIsPlayersConnected[i] = (Result == ERROR_SUCCESS);
			isConnected = isConnected || (Result == ERROR_SUCCESS);
		}

		if (isConnected)
		{

		}
	}

	void XBoxControllerHandler::Initialize()
	{}

	void XBoxControllerHandler::Update(WorldState& state)
	{
		// Obtain current gamepad state for all players
		UpdateState();
		(state);
		std::stringstream message;
		bool hasMessage = false;
		for (int i = 0; i < 4; ++i)
		{
			if (!LStick_InDeadzone(i))
			{
				if(!hasMessage)
					message << "Player " << (i + 1) << " has triggered: ";
				message << "LeftStick_X:" << LeftStick_X(i);
				hasMessage = true;
			}
		}

		if (hasMessage)
		{
			message << "\n";
			OutputDebugStringA(message.str().c_str());
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

#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include <XInput.h>


dae::InputManager::InputManager()
	: m_InputCommandMap(14, ControllerButtonHash, CompareControllerButton)
	, m_InputState(14, ControllerButtonHash, CompareControllerButton)
	, m_ControllerConnectedAtId{ false }
	, m_ControllerRegisteredAtId{ false }
{
	ProcessInput();
}

bool dae::InputManager::ProcessInput()
{
	//Iterate over all possible controllers
	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		const auto dwResult = XInputGetState(i, &state);

		// Check if a controller is connected at this index
		if (dwResult != ERROR_SUCCESS)
		{
			// If the controller was previously connected, fire callbacks
			if (m_ControllerConnectedAtId[i])
			{
				ControllerDisconnected();
				m_ControllerConnectedAtId[i] = false;
			}
			
			continue;
		}

		// If controller is newly connected, fire callbacks
		if (!m_ControllerConnectedAtId[i])
			ControllerConnected();
		
		m_ControllerConnectedAtId[i] = true;

		// Iterate over registered input actions and execute them when necessary
		for (auto& inputCommand : m_InputCommandMap)
		{
			if (inputCommand.first.ControllerId != i)
				continue;
			
			switch (inputCommand.second.eventType)
			{
			case EventType::pressed:
				if (m_InputState[inputCommand.first] == false && state.Gamepad.wButtons & inputCommand.first.Button)
					inputCommand.second.pCommand->Execute();
				break;
			case EventType::released:
				if (m_InputState[inputCommand.first] == true && !(state.Gamepad.wButtons & inputCommand.first.Button))
					inputCommand.second.pCommand->Execute();
				break;
			case EventType::down:
				if (state.Gamepad.wButtons & inputCommand.first.Button)
					inputCommand.second.pCommand->Execute();
			}

			m_InputState[inputCommand.first] = state.Gamepad.wButtons & inputCommand.first.Button;
		}
	}

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {

		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {

		}
	}

	return true;
}

DWORD dae::InputManager::RegisterController()
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		// Don't use m_ControllerConnectedAtId here! This function can be called before that array is properly loaded!
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		const auto dwResult = XInputGetState(i, &state);

		// Check if a controller is connected at this index
		if (dwResult != ERROR_SUCCESS)
			continue;

		if(!m_ControllerRegisteredAtId[i])
		{
			m_ControllerRegisteredAtId[i] = true;
			return i;
		}
	}

	return (DWORD)-1;
}

void dae::InputManager::UnregisterController(DWORD id)
{
	if (id >= XUSER_MAX_COUNT)
		return;
	
	m_ControllerRegisteredAtId[id] = false;
}

void dae::InputManager::AddInputAction(const ControllerButton& controllerButton, Command* pCommand, EventType eventType)
{
	m_InputCommandMap.insert(std::make_pair(controllerButton, InputAction{pCommand, eventType}));
}

size_t dae::InputManager::AddControllerConnectCallback(const std::function<void()>& callback)
{
	m_OnControllerConnectCallbacks.push_back(callback);
	return m_OnControllerConnectCallbacks.size() - 1;
}

void dae::InputManager::RemoveControllerConnectCallback(size_t id)
{
	m_OnControllerConnectCallbacks.erase(m_OnControllerConnectCallbacks.begin() + id);
}

size_t dae::InputManager::AddControllerDisconnectCallback(const std::function<void()>& callback)
{
	m_OnControllerDisconnectCallbacks.push_back(callback);
	return m_OnControllerDisconnectCallbacks.size() - 1;
}

void dae::InputManager::RemoveControllerDisconnectCallback(size_t id)
{
	m_OnControllerDisconnectCallbacks.erase(m_OnControllerDisconnectCallbacks.begin() + id);
}

size_t dae::InputManager::ControllerButtonHash(const ControllerButton& controllerButton)
{
	const auto hash = std::hash<WORD>();
	return hash(controllerButton.Button);
}

bool dae::InputManager::CompareControllerButton(const ControllerButton& cb1, const ControllerButton& cb2)
{
	return cb1.Button == cb2.Button && cb1.ControllerId == cb2.ControllerId;
}

void dae::InputManager::ControllerConnected() const
{
	for (auto& func : m_OnControllerConnectCallbacks)
		func();
}

void dae::InputManager::ControllerDisconnected() const
{
	for (auto& func : m_OnControllerDisconnectCallbacks)
		func();
}

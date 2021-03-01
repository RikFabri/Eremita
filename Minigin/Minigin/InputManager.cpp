#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include <XInput.h>


dae::InputManager::InputManager()
	: m_InputCommandMap(14, ControllerButtonHash, CompareControllerButton)
	, m_InputState(14, ControllerButtonHash, CompareControllerButton)
{
}

bool dae::InputManager::ProcessInput()
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		const auto dwResult = XInputGetState(i, &state);

		if (dwResult != ERROR_SUCCESS)
		{
			continue;
		}

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

void dae::InputManager::AddInputAction(const ControllerButton& controllerButton, Command* pCommand, EventType eventType)
{
	m_InputCommandMap.insert(std::make_pair(controllerButton, InputAction{pCommand, eventType}));
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

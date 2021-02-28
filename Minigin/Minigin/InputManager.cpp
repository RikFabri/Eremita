#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>


bool dae::InputManager::ProcessInput()
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	const auto dwResult = XInputGetState(0, &state);

	if (dwResult != ERROR_SUCCESS)
	{
		std::cerr << "no controller" << std::endl;
		return true;
	}

	for (auto& inputCommand : m_InputCommandMap)
	{
		switch (inputCommand.second.eventType)
		{
		case EventType::pressed:
			if (m_InputState[inputCommand.first] == false && state.Gamepad.wButtons & inputCommand.first)
				inputCommand.second.pCommand->Execute();
			break;
		case EventType::released:
			if (m_InputState[inputCommand.first] == true && !(state.Gamepad.wButtons & inputCommand.first))
				inputCommand.second.pCommand->Execute();
			break;
		case EventType::down:
			if (state.Gamepad.wButtons & inputCommand.first)
				inputCommand.second.pCommand->Execute();
		}

		m_InputState[inputCommand.first] = state.Gamepad.wButtons & inputCommand.first;
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

void dae::InputManager::AddInputAction(WORD keyBinding, Command* pCommand, EventType eventType)
{
	m_InputCommandMap.insert(std::make_pair(keyBinding, InputAction{pCommand, eventType}));
}

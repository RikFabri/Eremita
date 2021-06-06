#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include <XInput.h>
#include "Logger.h"


dae::InputManager::InputManager()
	: m_InputCommandMap(14, ControllerButtonHash, CompareControllerButton)
	, m_InputState(14, ControllerButtonHash, CompareControllerButton)
	, m_PhysicalControllerConnectedAtId{ false }
	, m_VirtualControllerRegisteredAtId{ false }
	, m_pSubjectComponent(std::make_unique<SubjectComponent>())
	, m_ConnectedControllers(0)
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
			if (m_PhysicalControllerConnectedAtId[i])
			{
				ControllerDisconnected();
				m_PhysicalControllerConnectedAtId[i] = false;
			}
			
			continue;
		}

		// If controller is newly connected, fire callbacks
		if (!m_PhysicalControllerConnectedAtId[i])
			ControllerConnected();
		
		m_PhysicalControllerConnectedAtId[i] = true;

		// Iterate over registered input actions and execute them when necessary
		for (auto& inputCommand : m_InputCommandMap)
		{
			CheckInputCommand(inputCommand, i, state);
		}
	}

	// No controllers are connected, update all input commands once for keyboard input
	if (m_ConnectedControllers <= 0)
	{
		for (auto& inputCommand : m_InputCommandMap)
		{
			// Passing dummy id & state
			XINPUT_STATE state{};
			CheckInputCommand(inputCommand, (DWORD)-1, state);
		}
	}

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
	}

	return true;
}

bool dae::InputManager::IsKeyDown(SDL_Keycode key)
{
	// Hastily added keyboard support
	const Uint8* keyState = SDL_GetKeyboardState(nullptr);

	return keyState[SDL_GetScancodeFromKey(key)];
}

void dae::InputManager::CheckInputCommand(std::pair<const dae::ControllerButton, dae::InputAction>& inputCommand, const DWORD& i, XINPUT_STATE& state)
{
	const auto useController = !inputCommand.first.KeyboardOnly;
	if (inputCommand.first.ControllerId != i && useController && inputCommand.first.ControllerId != (DWORD)-1)
	{
		return;
	}

	switch (inputCommand.second.eventType)
	{
	case EventType::pressed:
		if (m_InputState[inputCommand.first] == false && ((useController && state.Gamepad.wButtons & inputCommand.first.Button) ||  IsKeyDown(inputCommand.first.Key)))
			inputCommand.second.pCommand->Execute();
		break;
	case EventType::released:
		if (m_InputState[inputCommand.first] == true && !((useController && state.Gamepad.wButtons & inputCommand.first.Button) || IsKeyDown(inputCommand.first.Key)))
			inputCommand.second.pCommand->Execute();
		break;
	case EventType::down:
		if ((useController && state.Gamepad.wButtons & inputCommand.first.Button) || IsKeyDown(inputCommand.first.Key))
			inputCommand.second.pCommand->Execute();
	}

	m_InputState[inputCommand.first] = (useController && state.Gamepad.wButtons & inputCommand.first.Button) || IsKeyDown(inputCommand.first.Key);
}

DWORD dae::InputManager::RegisterController()
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		// Don't use m_PhysicalControllerConnectedAtId here! This function can be called before that array is properly loaded!
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		const auto dwResult = XInputGetState(i, &state);

		// Check if a controller is connected at this index
		if (dwResult != ERROR_SUCCESS)
			continue;

		if(!m_VirtualControllerRegisteredAtId[i])
		{
			m_VirtualControllerRegisteredAtId[i] = true;
			return i;
		}
	}

	return (DWORD)-1;
}

void dae::InputManager::UnregisterController(DWORD id)
{
	if (id >= XUSER_MAX_COUNT)
		return;
	
	m_VirtualControllerRegisteredAtId[id] = false;
}

dae::InputManager::inputActionIterator dae::InputManager::AddInputAction(const ControllerButton& controllerButton, Command* pCommand, EventType eventType)
{

	std::pair<inputActionIterator, bool> pair = m_InputCommandMap.insert(std::make_pair(controllerButton, InputAction{pCommand, eventType}));

	if(!pair.second)
		Logger::GetInstance().Print("Couldn't insert inputCommand");	

	return pair.first;
}

void dae::InputManager::RemoveInputAction(const ControllerButton& btn)
{
	m_InputCommandMap.erase(btn);
}

void dae::InputManager::SubscribeToControllerEvents(ObserverInterface* observer)
{
	m_pSubjectComponent->Subscribe(observer);
}

void dae::InputManager::UnSubscribeToControllerEvents(ObserverInterface* observer)
{
	m_pSubjectComponent->Unsubscribe(observer);
}

void dae::InputManager::Reset()
{
	m_InputCommandMap.clear();
	m_InputState.clear();

	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		m_VirtualControllerRegisteredAtId[i] = false;
		m_PhysicalControllerConnectedAtId[i] = false;
	}

	m_pSubjectComponent.reset(new SubjectComponent());

	// This repopulates the controller states
	ProcessInput();
}

size_t dae::InputManager::ControllerButtonHash(const ControllerButton& controllerButton)
{
	const auto hash = std::hash<WORD>();
	return hash(controllerButton.Button);
}

bool dae::InputManager::CompareControllerButton(const ControllerButton& cb1, const ControllerButton& cb2)
{
	return cb1.Button == cb2.Button && cb1.ControllerId == cb2.ControllerId && cb1.Key == cb2.Key && cb1.KeyboardOnly == cb2.KeyboardOnly;
}

void dae::InputManager::ControllerConnected()
{
	++m_ConnectedControllers;
	m_pSubjectComponent->Broadcast(nullptr, "Controller connected");
}

void dae::InputManager::ControllerDisconnected()
{
	--m_ConnectedControllers;
	m_pSubjectComponent->Broadcast(nullptr, "Controller disconnected");
}

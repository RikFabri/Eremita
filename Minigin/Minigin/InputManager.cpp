#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include <XInput.h>


dae::InputManager::InputManager()
	: m_InputCommandMap(14, ControllerButtonHash, CompareControllerButton)
	, m_InputState(14, ControllerButtonHash, CompareControllerButton)
	, m_PhysicalControllerConnectedAtId{ false }
	, m_VirtualControllerRegisteredAtId{ false }
	, m_pSubjectComponent(std::make_unique<SubjectComponent>())
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

void dae::InputManager::AddInputAction(const ControllerButton& controllerButton, Command* pCommand, EventType eventType)
{
	m_InputCommandMap.insert(std::make_pair(controllerButton, InputAction{pCommand, eventType}));
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
	return cb1.Button == cb2.Button && cb1.ControllerId == cb2.ControllerId;
}

void dae::InputManager::ControllerConnected() const
{
	m_pSubjectComponent->Broadcast(nullptr, "Controller connected");
}

void dae::InputManager::ControllerDisconnected() const
{
	m_pSubjectComponent->Broadcast(nullptr, "Controller disconnected");
}

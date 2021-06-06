#pragma once
#include <functional>
#include <unordered_map>
#include <windows.h>
#include <XInput.h>
#include "Commands.h"
#include "Singleton.h"
#include "SubjectComponent.h"
#include "SDL.h"

namespace dae
{
	struct ControllerButton
	{
		WORD Button = 0;
		DWORD ControllerId = 0;
		SDL_Keycode Key = SDLK_UNKNOWN;
		bool KeyboardOnly = false;
	};
		
	class InputManager final : public Singleton<InputManager>
	{
		using inputActionIterator = std::unordered_map<dae::ControllerButton, dae::InputAction,
			std::function<size_t(const dae::ControllerButton&)>,
			std::function<bool(const dae::ControllerButton&, const dae::ControllerButton&)>>::iterator;
	public:
		
		bool ProcessInput();

		//Returns controller id, or DWORD -1 (watch out, DWORD is unsigned) if no controller found
		DWORD RegisterController();
		void UnregisterController(DWORD id);
		
		// Adds an input action, or overwrites an existing one
		inputActionIterator AddInputAction(const ControllerButton& controllerButton, Command* pCommand, EventType eventType = EventType::released);
		void RemoveInputAction(const ControllerButton& btn);

		// Get notified when controllers connect/disconnect
		void SubscribeToControllerEvents(ObserverInterface* observer);
		// No longer be notified when a controller connects/disconnects
		void UnSubscribeToControllerEvents(ObserverInterface* observer);

		void Reset();
	private:

		friend class Singleton;
		InputManager();

		int m_ConnectedControllers;

		// This array keeps track of which controllers are in use
		bool m_VirtualControllerRegisteredAtId[XUSER_MAX_COUNT];
		// This array keeps track of the physically connected controllers
		bool m_PhysicalControllerConnectedAtId[XUSER_MAX_COUNT];

		void ControllerConnected();
		void ControllerDisconnected();
		
		static size_t ControllerButtonHash(const ControllerButton& controllerButton);
		static bool CompareControllerButton(const ControllerButton& cb1, const ControllerButton& cb2);

		bool IsKeyDown(SDL_Keycode key);
		void CheckInputCommand(std::pair<const dae::ControllerButton, dae::InputAction>& inputCommand, const DWORD& i, XINPUT_STATE& state);

		std::unordered_map<const ControllerButton, InputAction, std::function<size_t(const ControllerButton&)>, std::function<bool(const ControllerButton&, const ControllerButton&)>> m_InputCommandMap;
		std::unordered_map<const ControllerButton, bool, std::function<size_t(const ControllerButton&)>, std::function<bool(const ControllerButton&, const ControllerButton&)>> m_InputState;

		std::unique_ptr<SubjectComponent> m_pSubjectComponent;
	};
}

#pragma once
#include <functional>
#include <unordered_map>
#include <windows.h>
#include <XInput.h>
#include "Commands.h"
#include "Singleton.h"


namespace dae
{
	struct ControllerButton
	{
		WORD Button = 0;
		DWORD ControllerId = 0;
	};
		
	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		
		bool ProcessInput();

		//Returns controller id, or DWORD -1 (watch out, DWORD is unsigned) if no controller found
		DWORD RegisterController();
		void UnregisterController(DWORD id);
		
		void AddInputAction(const ControllerButton& controllerButton, Command* pCommand, EventType eventType = EventType::released);
	private:
		bool m_ControllerRegistered[XUSER_MAX_COUNT];
		
		static size_t ControllerButtonHash(const ControllerButton& controllerButton);
		static bool CompareControllerButton(const ControllerButton& cb1, const ControllerButton& cb2);


		std::unordered_map<ControllerButton, InputAction, std::function<size_t(const ControllerButton&)>, std::function<bool(const ControllerButton&, const ControllerButton&)>> m_InputCommandMap;
		std::unordered_map<ControllerButton, bool, std::function<size_t(const ControllerButton&)>, std::function<bool(const ControllerButton&, const ControllerButton&)>> m_InputState;
	};

}

#pragma once
#include <functional>
#include <unordered_map>
#include <windows.h>
#include <XInput.h>
#include "Commands.h"
#include "Singleton.h"
#include "SubjectComponent.h"


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
		
		bool ProcessInput();

		//Returns controller id, or DWORD -1 (watch out, DWORD is unsigned) if no controller found
		DWORD RegisterController();
		void UnregisterController(DWORD id);
		
		void AddInputAction(const ControllerButton& controllerButton, Command* pCommand, EventType eventType = EventType::released);

		// Get notified when controllers connect/disconnect
		void SubscribeToControllerEvents(ObserverInterface* observer);
		// No longer be notified when a controller connects/disconnects
		void UnSubscribeToControllerEvents(ObserverInterface* observer);

	private:
		friend class Singleton;
		InputManager();


		// This array keeps track of which controllers are in use
		bool m_VirtualControllerRegisteredAtId[XUSER_MAX_COUNT];
		// This array keeps track of the physically connected controllers
		bool m_PhysicalControllerConnectedAtId[XUSER_MAX_COUNT];

		// -------------------- Observer pattern fits better here ------------- //
		void ControllerConnected() const;
		void ControllerDisconnected() const;
		// -------------------------------------------------------------------- //
		
		static size_t ControllerButtonHash(const ControllerButton& controllerButton);
		static bool CompareControllerButton(const ControllerButton& cb1, const ControllerButton& cb2);


		std::unordered_map<ControllerButton, InputAction, std::function<size_t(const ControllerButton&)>, std::function<bool(const ControllerButton&, const ControllerButton&)>> m_InputCommandMap;
		std::unordered_map<ControllerButton, bool, std::function<size_t(const ControllerButton&)>, std::function<bool(const ControllerButton&, const ControllerButton&)>> m_InputState;

		std::unique_ptr<SubjectComponent> m_pSubjectComponent;
	};
}

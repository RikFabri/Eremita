#pragma once
#include <unordered_map>
#include <XInput.h>

#include "Commands.h"
#include "Singleton.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:		
		bool ProcessInput();

		void AddInputAction(WORD keyBinding, Command* pCommand, EventType eventType);
	private:
		std::unordered_map<WORD, InputAction> m_InputCommandMap;
		std::unordered_map<WORD, bool> m_InputState;
	};

}

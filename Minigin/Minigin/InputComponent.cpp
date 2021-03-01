#include "InputComponent.h"
#include "InputManager.h"
#include <string>


#include "HealthComponent.h"
#include "SceneObject.h"
#include "ScoreComponent.h"
#include "Logger.h"

dae::InputComponent::InputComponent()
	: m_ControllerId(0)
{
}

dae::InputComponent::~InputComponent()
{
	InputManager::GetInstance().UnregisterController(m_ControllerId);
}

void dae::InputComponent::Init(SceneObject& parent)
{
	m_ControllerId = InputManager::GetInstance().RegisterController();

	if (m_ControllerId == (DWORD)-1)
	{
		//const auto message = "Couldn't register controller, \ndid you forget to plug it in or exceed " + std::to_string(XUSER_MAX_COUNT) + " Controllers?";
		//Logger::GetInstance().Print(message);

		InputManager::GetInstance().AddControllerConnectCallback([this, &parent]() {Init(parent); });
		
		return;
	}

	m_pScoreComponentRef = parent.GetFirstComponentOfType<ScoreComponent>();
	m_pHealthComponentRef = parent.GetFirstComponentOfType<HealthComponent>();
	
	InputManager::GetInstance().AddInputAction(
	{XINPUT_GAMEPAD_DPAD_LEFT, m_ControllerId}, new ExecuteFunction([this]()
	{
			m_pScoreComponentRef->IncreaseScore(25);
	}));
	
	InputManager::GetInstance().AddInputAction(
	{XINPUT_GAMEPAD_DPAD_UP, m_ControllerId}, new ExecuteFunction([this]()
	{
			m_pScoreComponentRef->IncreaseScore(50);
	}));
	
	InputManager::GetInstance().AddInputAction(
	{XINPUT_GAMEPAD_DPAD_RIGHT, m_ControllerId}, new ExecuteFunction([this]()
	{
			m_pScoreComponentRef->IncreaseScore(300);
	}));
	
	InputManager::GetInstance().AddInputAction(
	{XINPUT_GAMEPAD_DPAD_DOWN, m_ControllerId}, new ExecuteFunction([this]()
	{
			m_pScoreComponentRef->IncreaseScore(500);
	}));
	
	InputManager::GetInstance().AddInputAction(ControllerButton{ XINPUT_GAMEPAD_A, m_ControllerId }, new ExecuteFunction([this]()
	{
			m_pHealthComponentRef->Die();
	}));

}

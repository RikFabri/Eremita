#include "InputComponent.h"

#include <iostream>
#include <SDL.h>


#include "InputManager.h"
#include <string>


#include "HealthComponent.h"
#include "SceneObject.h"
#include "ScoreComponent.h"
#include "Logger.h"
#include "SubjectComponent.h"
#include "SoundServiceLocator.h"
#include <thread>

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

	// If there was no controller to subscribe to, add the initialization as a callback
	if (m_ControllerId == (DWORD)-1)
	{
		InputManager::GetInstance().AddControllerConnectCallback([this, &parent]()
		{
			Init(parent);

			Logger::GetInstance().Print("Player " + std::to_string(m_ControllerId + 1) + " joined");
			
			auto* subject = parent.GetFirstComponentOfType<SubjectComponent>();
			auto* pScoreComp = parent.GetFirstComponentOfType<ScoreComponent>();
			auto* pHealthComp = parent.GetFirstComponentOfType<HealthComponent>();
			if (!subject)
			{
				Logger::GetInstance().Print("no subject found");
				return;
			}
			if (!pScoreComp)
			{
				Logger::GetInstance().Print("no scoreComp found");
				return;
			}
			//We want to fake a score update so that possibly invisible score displays update
			subject->Broadcast(pScoreComp, "UpdateScore");
			//We want to fake a change in health so that possibly invisible health displays update
			subject->Broadcast(pHealthComp, "UpdateHealth");
		});
		
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

	InputManager::GetInstance().AddInputAction(ControllerButton{ XINPUT_GAMEPAD_B, m_ControllerId }, new ExecuteFunction([this]()
	{
			auto* const service = SoundServiceLocator::GetSoundService();
			service->PlaySound("../Data/door2.wav", SDL_MIX_MAXVOLUME);
	}));

	InputManager::GetInstance().AddInputAction(ControllerButton{ XINPUT_GAMEPAD_Y, m_ControllerId }, new ExecuteFunction([this]()
	{
			SoundServiceLocator::GetSoundService()->StopAllSounds();
	}));

	InputManager::GetInstance().AddInputAction(ControllerButton{ XINPUT_GAMEPAD_X, m_ControllerId }, new ExecuteFunction([this]()
	{
			auto* const service = SoundServiceLocator::GetSoundService();
			service->SetMuted(!service->IsMuted());
	}));
}

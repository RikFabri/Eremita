#include "InputComponent.h"

#include <iostream>
#include <SDL.h>


#include "InputManager.h"
#include <string>

#include "QBertBehaviourComponent.h"
#include "HealthComponent.h"
#include "SceneObject.h"
#include "ScoreComponent.h"
#include "Logger.h"
#include "SubjectComponent.h"
#include "SoundServiceLocator.h"
#include <thread>

#include "RenderComponent.h"
#include "DestroyOnPlayerDamageComponent.h"
#include "TimerComponent.h"
#include "SlickAndSamBehaviourComponent.h"
#include "DefaultMovement.h"
#include "PosessedMovementComponent.h"
#include "Scene.h"


using namespace dae;

InputComponent::InputComponent()
	: m_ControllerId(0)
	, m_RegisteredCallback(false)
	, m_pParentRef(nullptr)
	, m_pMovementCompRef(nullptr)
{
}

InputComponent::~InputComponent()
{
	InputManager::GetInstance().UnregisterController(m_ControllerId);
}

void InputComponent::Init(SceneObject& parent)
{
	m_pParentRef = &parent;
	m_ControllerId = InputManager::GetInstance().RegisterController();

	// If there was no controller to subscribe to subscribe to controller events and initialize whenever possible
	if (m_ControllerId == (DWORD)-1)
	{
		if (m_RegisteredCallback)
			return;

		InputManager::GetInstance().SubscribeToControllerEvents(this);
		m_RegisteredCallback = true;
		return;
	}

	m_pMovementCompRef = parent.GetFirstComponentOfType<PosessedMovementComponent>();
	m_pMovementCompRef->SetEnabled(true);
	
	InputManager::GetInstance().AddInputAction(
	{XINPUT_GAMEPAD_DPAD_LEFT, m_ControllerId}, new ExecuteFunction([this]()
	{
			m_pMovementCompRef->Move(-1, -1);
	}));
	
	InputManager::GetInstance().AddInputAction(
	{XINPUT_GAMEPAD_DPAD_UP, m_ControllerId}, new ExecuteFunction([this]()
	{
			m_pMovementCompRef->Move(0, -1);
	}));
	
	InputManager::GetInstance().AddInputAction(
	{XINPUT_GAMEPAD_DPAD_RIGHT, m_ControllerId}, new ExecuteFunction([this]()
	{
			m_pMovementCompRef->Move(1, 1);
	}));
	
	InputManager::GetInstance().AddInputAction(
	{XINPUT_GAMEPAD_DPAD_DOWN, m_ControllerId}, new ExecuteFunction([this]()
	{
			m_pMovementCompRef->Move(0, 1);
	}));
}

void InputComponent::OnNotify(const BaseComponent*, const std::string& message)
{
	if (message == "Controller connected")
	{
		Init(*m_pParentRef);
		if (m_ControllerId == (DWORD)-1)
			return;
		Logger::GetInstance().Print("Player " + std::to_string(m_ControllerId) + " joined");

		InputManager::GetInstance().UnSubscribeToControllerEvents(this);
	}
}

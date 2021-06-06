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

InputComponent::InputComponent(bool useWASD)
	: m_ControllerId(0)
	, m_RegisteredCallback(false)
	, m_pParentRef(nullptr)
	, m_pMovementCompRef(nullptr)
	, m_UseWASD(useWASD)
{
}

InputComponent::~InputComponent()
{
	InputManager::GetInstance().UnregisterController(m_ControllerId);
}

void InputComponent::Init(SceneObject& parent)
{
	auto destroyOnDmg = parent.GetFirstComponentOfType<DestroyOnPlayerDamageComponent>();

	if (destroyOnDmg)
		destroyOnDmg->SetOnDeath(std::bind(&InputComponent::RemoveConfig, this));

	m_pParentRef = &parent;
	m_ControllerId = InputManager::GetInstance().RegisterController();

	// If there was no controller to subscribe to subscribe to controller events and initialize whenever possible
	if (m_ControllerId == (DWORD)-1)
	{
		if (m_RegisteredCallback)
			return;

		// Sets input config for keyboard usage, not tied to controllerId
		SetInputConfig(true);

		InputManager::GetInstance().SubscribeToControllerEvents(this);
		m_RegisteredCallback = true;
		return;
	}

	// Resets input config with correct controller id
	SetInputConfig(false);
}

void InputComponent::SetInputConfig(bool keyboardOnly)
{
	m_pMovementCompRef = m_pParentRef->GetFirstComponentOfType<PosessedMovementComponent>();
	m_pMovementCompRef->SetEnabled(true);

	RemoveConfig();

	auto Id = InputManager::GetInstance().AddInputAction(
		{ XINPUT_GAMEPAD_DPAD_LEFT, m_ControllerId, m_UseWASD ? SDLK_a : SDLK_LEFT, keyboardOnly }, new ExecuteFunction([this]()
			{
				m_pMovementCompRef->Move(-1, -1);
			}))->first;
	m_InputActionIds.push_back(Id);

	Id = InputManager::GetInstance().AddInputAction(
		{ XINPUT_GAMEPAD_DPAD_UP, m_ControllerId, m_UseWASD ? SDLK_w : SDLK_UP, keyboardOnly }, new ExecuteFunction([this]()
			{
				m_pMovementCompRef->Move(0, -1);
			}))->first;
	m_InputActionIds.push_back(Id);

	Id = InputManager::GetInstance().AddInputAction(
		{ XINPUT_GAMEPAD_DPAD_RIGHT, m_ControllerId, m_UseWASD ? SDLK_d : SDLK_RIGHT, keyboardOnly }, new ExecuteFunction([this]()
			{
				m_pMovementCompRef->Move(1, 1);
			}))->first;
	m_InputActionIds.push_back(Id);

	Id = InputManager::GetInstance().AddInputAction(
		{ XINPUT_GAMEPAD_DPAD_DOWN, m_ControllerId, m_UseWASD ? SDLK_s : SDLK_DOWN, keyboardOnly }, new ExecuteFunction([this]()
			{
				m_pMovementCompRef->Move(0, 1);
			}))->first;
	m_InputActionIds.push_back(Id);

}

void InputComponent::RemoveConfig()
{
	// Remove previous configuration
	for (const auto& btn : m_InputActionIds)
	{
		InputManager::GetInstance().RemoveInputAction(btn);
	}
	m_InputActionIds.clear();
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

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
#include "Scene.h"


using namespace dae;

InputComponent::InputComponent()
	: m_ControllerId(0)
	, m_RegisteredCallback(false)
	, m_pHealthComponentRef(nullptr)
	, m_pScoreComponentRef(nullptr)
	, m_pParentRef(nullptr)
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

	m_pScoreComponentRef = parent.GetFirstComponentOfType<ScoreComponent>();
	m_pHealthComponentRef = parent.GetFirstComponentOfType<HealthComponent>();
	m_pQbertBehaviour = parent.GetFirstComponentOfType<QBertBehaviourComponent>();
	
	InputManager::GetInstance().AddInputAction(
	{XINPUT_GAMEPAD_DPAD_LEFT, m_ControllerId}, new ExecuteFunction([this]()
	{
			//m_pScoreComponentRef->IncreaseScore(25);
			m_pQbertBehaviour->Move(-1, -1);
	}));
	
	InputManager::GetInstance().AddInputAction(
	{XINPUT_GAMEPAD_DPAD_UP, m_ControllerId}, new ExecuteFunction([this]()
	{
			//m_pScoreComponentRef->IncreaseScore(50);
			m_pQbertBehaviour->Move(0, -1);
	}));
	
	InputManager::GetInstance().AddInputAction(
	{XINPUT_GAMEPAD_DPAD_RIGHT, m_ControllerId}, new ExecuteFunction([this]()
	{
			//m_pScoreComponentRef->IncreaseScore(300);
			m_pQbertBehaviour->Move(1, 1);
	}));
	
	InputManager::GetInstance().AddInputAction(
	{XINPUT_GAMEPAD_DPAD_DOWN, m_ControllerId}, new ExecuteFunction([this]()
	{
			//m_pScoreComponentRef->IncreaseScore(500);
			m_pQbertBehaviour->Move(0, 1);
	}));
	
	InputManager::GetInstance().AddInputAction(ControllerButton{ XINPUT_GAMEPAD_A, m_ControllerId }, new ExecuteFunction([this]()
	{
			m_pHealthComponentRef->Die();
	}));

	InputManager::GetInstance().AddInputAction(ControllerButton{ XINPUT_GAMEPAD_B, m_ControllerId }, new ExecuteFunction([this, &parent]()
	{
			auto* const service = SoundServiceLocator::GetSoundService();
			service->PlaySound("../Data/door2.wav", SDL_MIX_MAXVOLUME);

			using pComponentVec = std::vector<BaseComponent*>;


			// Sam
			const auto samRenderer = new RenderComponent("Sam.png", { 16, -16 });
			const auto destroySamOnReset = new DestroyOnPlayerDamageComponent();
			const auto samTimer = new TimerComponent(1);
			const auto samBehaviour = new SlickAndSamBehaviourComponent();
			const auto samMovement = new DefaultMovement(false, true, true);
			const auto sam = std::make_shared<SceneObject>(pComponentVec{ samTimer, samMovement, samBehaviour, destroySamOnReset }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "sam");
			sam->AddComponent(samRenderer, true);
			parent.GetScene()->AddAfterInitialize(sam);
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

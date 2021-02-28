#include "HealthComponent.h"
#include "SubjectComponent.h"

#include "SceneObject.h"
#include "GameTime.h"
#include "InputManager.h"
#include <Xinput.h>

dae::HealthComponent::HealthComponent(int nrOfLives)
	: m_Lives(nrOfLives)
	, m_pBroadcasterRef(nullptr)
{
	
}

void dae::HealthComponent::Init(SceneObject& sceneObject)
{
	m_pBroadcasterRef = sceneObject.GetFirstComponentOfType<SubjectComponent>();

	
	InputManager::GetInstance().AddInputAction(ControllerButton{ XINPUT_GAMEPAD_A, 0 }, new ExecuteFunction([this]() {DoDamage(1); }), EventType::released);
}

void dae::HealthComponent::DoDamage(int amount)
{
	m_Lives -= amount;

	Die();
	
	//if (m_Lives <= 0)
	//	Die();
}

void dae::HealthComponent::Die()
{
	if (m_pBroadcasterRef)
		m_pBroadcasterRef->Broadcast(this, "Died");
}

int dae::HealthComponent::GetLives() const
{
	return m_Lives;
}
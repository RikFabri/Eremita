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
}

void dae::HealthComponent::Die()
{
	--m_Lives;
	
	if (m_pBroadcasterRef)
		m_pBroadcasterRef->Broadcast(this, "Died");
}

int dae::HealthComponent::GetLives() const
{
	return m_Lives;
}
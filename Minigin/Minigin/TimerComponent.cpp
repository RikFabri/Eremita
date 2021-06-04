#include "TimerComponent.h"
#include "GameTime.h"

dae::TimerComponent::TimerComponent(float waitTime)
	: m_WaitTime(waitTime)
	, m_PassedTime(0)
	, m_TimerFinished(false)
{
}

void dae::TimerComponent::Update(SceneObject&)
{
	m_PassedTime += (float)GameTime::GetInstance().GetDeltaTime();

	if (m_PassedTime >= m_WaitTime)
		m_TimerFinished = true;
}

void dae::TimerComponent::Reset()
{
	m_TimerFinished = false;
	m_PassedTime = 0;
}

bool dae::TimerComponent::TimerCompleted() const
{
	return m_TimerFinished;
}

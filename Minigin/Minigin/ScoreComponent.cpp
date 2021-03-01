#include "ScoreComponent.h"

#include "SceneObject.h"
#include "SubjectComponent.h"
#include "InputManager.h"
#include <XInput.h>

dae::ScoreComponent::ScoreComponent()
	: m_Score(0)
{
}

void dae::ScoreComponent::Init(SceneObject& parent)
{
	m_SubjectComponentRef = parent.GetFirstComponentOfType<SubjectComponent>();

	InputManager::GetInstance().AddInputAction(ControllerButton{ XINPUT_GAMEPAD_B, 0 }, new ExecuteFunction([this]() {IncreaseScore(1); }), EventType::released);

}

void dae::ScoreComponent::IncreaseScore(int amount)
{
	m_Score += amount;

	if (m_SubjectComponentRef)
		m_SubjectComponentRef->Broadcast(this, "ScoreIncreased");
}

int dae::ScoreComponent::GetScore() const
{
	return m_Score;
}

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

	if (m_SubjectComponentRef)
		m_SubjectComponentRef->Broadcast(this, "UpdateScore");
}

void dae::ScoreComponent::IncreaseScore(int amount)
{
	m_Score += amount;

	if (m_SubjectComponentRef)
		m_SubjectComponentRef->Broadcast(this, "UpdateScore");
}

int dae::ScoreComponent::GetScore() const
{
	return m_Score;
}

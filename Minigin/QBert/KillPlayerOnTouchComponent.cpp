#include "KillPlayerOnTouchComponent.h"
#include "QBertBehaviourComponent.h"
#include "SceneObject.h"
#include "Scene.h"

void KillPlayerOnTouchComponent::Init(dae::SceneObject& parent)
{
	const auto players = parent.GetScene()->GetObjectsByTag("player");
	m_QBertRef = players[0];
}

void KillPlayerOnTouchComponent::Update(dae::SceneObject& parent)
{
	const int killDistance = 8;

	if (m_QBertRef.expired())
		return;

	const auto qbert = m_QBertRef.lock();
	const auto qbertPos = qbert->GetTransform()->GetPosition();
	const auto pos = parent.GetTransform()->GetPosition();


	const auto coilyToQbertDistance = glm::distance(qbertPos, pos);

	if (coilyToQbertDistance < killDistance)
	{
		const auto qbertBehaviour = qbert->GetFirstComponentOfType<QBertBehaviourComponent>();
		qbertBehaviour->Damage();
	}

}

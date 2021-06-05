#include "KillPlayerOnTouchComponent.h"
#include "QBertBehaviourComponent.h"
#include "SceneObject.h"
#include "Scene.h"

void KillPlayerOnTouchComponent::Init(dae::SceneObject& parent)
{
	const auto players = parent.GetScene()->GetObjectsByTag("player");
	std::copy(players.begin(), players.end(), std::back_inserter(m_QBertRefs));
}

void KillPlayerOnTouchComponent::Update(dae::SceneObject& parent)
{
	const int killDistance = 8;

	for (auto& pQBertRef : m_QBertRefs)
	{
		if (pQBertRef.expired())
			return;

		const auto qbert = pQBertRef.lock();
		const auto qbertPos = qbert->GetTransform()->GetPosition();
		const auto pos = parent.GetTransform()->GetPosition();


		const auto coilyToQbertDistance = glm::distance(qbertPos, pos);

		if (coilyToQbertDistance < killDistance)
		{
			const auto qbertBehaviour = qbert->GetFirstComponentOfType<QBertBehaviourComponent>();
			qbertBehaviour->Damage();
		}
	}
}

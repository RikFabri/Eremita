#include "SlickAndSamBehaviourComponent.h"
#include "DefaultMovement.h"
#include "SceneObject.h"
#include "Scene.h"

void SlickAndSamBehaviourComponent::Init(dae::SceneObject& parent)
{
	const auto players = parent.GetScene()->GetObjectsByTag("player");
	std::copy(players.begin(), players.end(), std::back_inserter(m_QBertRefs));

	// Die when the end of the map is reached
	parent.GetFirstComponentOfType<DefaultMovement>()->SetReachedEnd([](dae::SceneObject& parent)
		{
			parent.GetScene()->Remove(&parent);
		});
}

void SlickAndSamBehaviourComponent::Update(dae::SceneObject& parent)
{
	DieIfQbertClose(parent);
}

void SlickAndSamBehaviourComponent::DieIfQbertClose(dae::SceneObject& parent)
{
	const int killDistance = 8;

	for (auto& pQBertRef : m_QBertRefs)
	{
		if (pQBertRef.expired())
			return;

		const auto qbert = pQBertRef.lock();
		const auto qbertPos = qbert->GetTransform()->GetPosition();
		const auto pos = parent.GetTransform()->GetPosition();


		const auto distanceToQbert = glm::distance(qbertPos, pos);

		if (distanceToQbert < killDistance)
		{
			parent.GetScene()->Remove(&parent);
		}
	}
}

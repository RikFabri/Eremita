#include "DestroyOnPlayerDamageComponent.h"
#include "SubjectComponent.h"
#include "SceneObject.h"
#include "Scene.h"


void DestroyOnPlayerDamageComponent::Init(dae::SceneObject& parent)
{
	m_pParentRef = &parent;

	const auto players = parent.GetScene()->GetObjectsByTag("player");
	for (const auto player : players)
	{
		const auto subject = player->GetFirstComponentOfType<dae::SubjectComponent>();
		subject->Subscribe(this);
	}
}

void DestroyOnPlayerDamageComponent::OnNotify(const BaseComponent*, const std::string& message)
{
	if (message == "UpdateHealth")
	{
		const auto players = m_pParentRef->GetScene()->GetObjectsByTag("player");
		for (const auto player : players)
		{
			const auto subject = player->GetFirstComponentOfType<dae::SubjectComponent>();
			subject->Unsubscribe(this);
		}

		m_pParentRef->GetScene()->Remove(m_pParentRef);
	}
}

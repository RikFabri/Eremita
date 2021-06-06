#include "DestroyOnPlayerDamageComponent.h"
#include "SubjectComponent.h"
#include "SceneObject.h"
#include "Logger.h"
#include "Scene.h"


DestroyOnPlayerDamageComponent::~DestroyOnPlayerDamageComponent()
{
	UnSubscribeFromAllPlayers();
}

void DestroyOnPlayerDamageComponent::Init(dae::SceneObject& parent)
{
	m_pParentRef = &parent;

	const auto players = parent.GetScene()->GetObjectsByTag("player");
	for (const auto player : players)
	{
		const auto subject = player->GetFirstComponentOfType<dae::SubjectComponent>();
		subject->Subscribe(this);
		m_pSubjects.push_back(player);
	}
}

void DestroyOnPlayerDamageComponent::OnNotify(const BaseComponent*, const std::string& message)
{
	if (message == "UpdateHealth")
	{
		if (m_OnDeath)
			m_OnDeath();

		UnSubscribeFromAllPlayers();

		m_pParentRef->GetScene()->Remove(m_pParentRef);
	}
}

void DestroyOnPlayerDamageComponent::SetOnDeath(const std::function<void()>& callback)
{
	m_OnDeath = callback;
}

void DestroyOnPlayerDamageComponent::UnSubscribeFromAllPlayers()
{
	if (m_pSubjects.empty())
		return;
	
	for (auto& subject : m_pSubjects)
	{
		if (subject.expired())
		{
#if _DEBUG
			dae::Logger::GetInstance().Print("couldn't unsubscribe");
			dae::Logger::GetInstance().SaveLog("Log.txt");
#endif // _DEBUG
			continue;
		}

		const auto subjectComp = subject.lock()->GetFirstComponentOfType<dae::SubjectComponent>();
		if(subjectComp)
			subjectComp->Unsubscribe(this);
	}
	m_pSubjects.clear();
}

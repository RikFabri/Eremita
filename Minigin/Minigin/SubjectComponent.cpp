#include "SubjectComponent.h"

dae::SubjectComponent::SubjectComponent()
	: m_IsNotifying(false)
{
}

void dae::SubjectComponent::Subscribe(ObserverInterface* pObserver)
{
	m_pObservers.push_back(pObserver);
}

void dae::SubjectComponent::Unsubscribe(ObserverInterface* pObserver)
{
	if (!m_IsNotifying)
		m_pObservers.erase(std::find(m_pObservers.begin(), m_pObservers.end(), pObserver));
	else
	{
		// Don't remove observers while broadcasting
		m_pObserversToRemove.push_back(pObserver);
	}
}

void dae::SubjectComponent::Broadcast(BaseComponent* pComponent, const std::string& message)
{
	m_IsNotifying = true;
	for(auto* pObserver : m_pObservers)
	{
		pObserver->OnNotify(pComponent, message);
	}

	if (!m_pObserversToRemove.empty())
	{
		auto endIt = m_pObservers.end();
		for (auto observer : m_pObserversToRemove)
		{
			endIt = std::remove(m_pObservers.begin(), endIt, observer);
		}
		m_pObservers.erase(m_pObservers.begin(), endIt);
		m_pObserversToRemove.clear();
	}
}

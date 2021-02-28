#include "SubjectComponent.h"

void dae::SubjectComponent::Subscribe(ObserverInterface* pObserver)
{
	m_pObservers.push_back(pObserver);
}

void dae::SubjectComponent::Unsubscribe(ObserverInterface* pObserver)
{
	m_pObservers.erase(std::find(m_pObservers.begin(), m_pObservers.end(), pObserver));
}

void dae::SubjectComponent::Broadcast(BaseComponent* pComponent, const std::string& message)
{
	for(auto* pObserver : m_pObservers)
	{
		pObserver->OnNotify(pComponent, message);
	}
}

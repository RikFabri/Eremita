#pragma once
#include <vector>

#include "BaseComponent.h"
#include "ObserverInterface.h"

namespace dae
{
	class SubjectComponent final : public BaseComponent
	{
	public:
		void Subscribe(ObserverInterface* pObserver);
		void Unsubscribe(ObserverInterface* pObserver);
		void Broadcast(BaseComponent* pComponent, const std::string& message);
	
	private:
		std::vector<ObserverInterface*> m_pObservers;
	};
}

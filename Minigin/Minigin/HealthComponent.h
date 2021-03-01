#pragma once
#include "BaseComponent.h"

namespace dae
{
	class SubjectComponent;
	
	class HealthComponent final : public BaseComponent
	{
	public:
		HealthComponent(int nrOfLives = 3);
		
		void Init(SceneObject&) override;
		
		void Die();

		int GetLives() const;
	
	private:
		int m_Lives;
		
		//Not owned
		SubjectComponent* m_pBroadcasterRef;
	};
}

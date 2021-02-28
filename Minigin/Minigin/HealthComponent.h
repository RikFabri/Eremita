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

		void DoDamage(int amount);
		void Die();

		int GetLives() const;
		void Update(SceneObject&) override;
	private:
		int m_Lives;
		float m_ElapsedSec = 0;
		
		//Not owned
		SubjectComponent* m_pBroadcasterRef;
	};
}

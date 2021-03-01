#pragma once
#include "BaseComponent.h"
#include <Windows.h>

namespace dae
{
	class ScoreComponent;
	class HealthComponent;
	
	class InputComponent final : public BaseComponent
	{
	public:
		InputComponent();
		~InputComponent();
		
		void Init(SceneObject& parent) override;
	
	private:
		DWORD m_ControllerId;

		//No ownership
		ScoreComponent* m_pScoreComponentRef;
		HealthComponent* m_pHealthComponentRef;
	};
}
#pragma once
#include "BaseComponent.h"
#include <string>
#include <Windows.h>
#include "ObserverInterface.h"

namespace dae
{
	class ScoreComponent;
	class HealthComponent;
	
	class InputComponent final : public BaseComponent, public ObserverInterface
	{
	public:
		InputComponent();
		~InputComponent();
		
		void Init(SceneObject& parent) override;
	
	private:
		DWORD m_ControllerId;
		bool m_RegisteredCallback;

		//No ownership
		SceneObject* m_pParentRef;
		ScoreComponent* m_pScoreComponentRef;
		HealthComponent* m_pHealthComponentRef;

		virtual void OnNotify(const BaseComponent*, const std::string& message) override;
	};
}
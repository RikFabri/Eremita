#pragma once
#include "BaseComponent.h"
#include <string>
#include <Windows.h>
#include "ObserverInterface.h"

namespace dae {
	class ScoreComponent;
	class HealthComponent;
}

class QBertBehaviourComponent;

class InputComponent final : public dae::BaseComponent, public dae::ObserverInterface
{
public:
	InputComponent();
	~InputComponent();
		
	void Init(dae::SceneObject& parent) override;
	
private:
	DWORD m_ControllerId;
	bool m_RegisteredCallback;

	//No ownership
	dae::SceneObject* m_pParentRef;
	dae::ScoreComponent* m_pScoreComponentRef;
	dae::HealthComponent* m_pHealthComponentRef;
	QBertBehaviourComponent* m_pQbertBehaviour;

	virtual void OnNotify(const BaseComponent*, const std::string& message) override;
};

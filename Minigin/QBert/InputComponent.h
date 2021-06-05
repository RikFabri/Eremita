#pragma once
#include "BaseComponent.h"
#include <string>
#include <Windows.h>
#include "ObserverInterface.h"

namespace dae {
	class ScoreComponent;
	class HealthComponent;
}

class PosessedMovementComponent;

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
	PosessedMovementComponent* m_pMovementCompRef;

	virtual void OnNotify(const BaseComponent*, const std::string& message) override;
};

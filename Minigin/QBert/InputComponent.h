#pragma once
#include "BaseComponent.h"
#include <string>
#include <Windows.h>
#include "ObserverInterface.h"
#include <vector>

namespace dae {
	class ScoreComponent;
	class HealthComponent;
	struct ControllerButton;
}

class PosessedMovementComponent;

class InputComponent final : public dae::BaseComponent, public dae::ObserverInterface
{
public:
	InputComponent(bool useWASD = true);
	~InputComponent();
		
	void Init(dae::SceneObject& parent) override;
	
private:
	DWORD m_ControllerId;
	bool m_RegisteredCallback;
	bool m_UseWASD;

	std::vector<dae::ControllerButton> m_InputActionIds;

	void SetInputConfig(bool keyboardOnly = false);

	//No ownership
	dae::SceneObject* m_pParentRef;
	PosessedMovementComponent* m_pMovementCompRef;

	virtual void OnNotify(const BaseComponent*, const std::string& message) override;
};

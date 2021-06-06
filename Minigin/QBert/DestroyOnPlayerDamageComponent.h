#pragma once
#include <BaseComponent.h>
#include "ObserverInterface.h"
#include <functional>
#include <memory>
#include <vector>

namespace dae
{
	class Scene;
}

class DestroyOnPlayerDamageComponent : public dae::BaseComponent, public dae::ObserverInterface
{
public:
	~DestroyOnPlayerDamageComponent();

	virtual void Init(dae::SceneObject& parent) override;

	virtual void OnNotify(const BaseComponent*, const std::string& message) override;
	void SetOnDeath(const std::function<void()>& callback);
private:
	std::function<void()> m_OnDeath;
	void UnSubscribeFromAllPlayers();

	// No ownership
	dae::SceneObject* m_pParentRef;
	std::vector<std::weak_ptr<dae::SceneObject>> m_pSubjects;
};


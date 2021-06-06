#pragma once
#include <BaseComponent.h>
#include "ObserverInterface.h"
#include <memory>


class DestroyOnLevelChange : public dae::BaseComponent, public dae::ObserverInterface
{
public:
	~DestroyOnLevelChange();
	virtual void Init(dae::SceneObject& parent) override;

	virtual void OnNotify(const BaseComponent*, const std::string& message) override;

private:

	// No ownership
	dae::SceneObject* m_pParentRef;
	std::weak_ptr<dae::SceneObject> m_pTileMapRef;
};


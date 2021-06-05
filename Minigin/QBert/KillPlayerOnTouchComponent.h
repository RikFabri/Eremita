#pragma once
#include <BaseComponent.h>
#include <memory>


class KillPlayerOnTouchComponent : public dae::BaseComponent
{
public:
	virtual void Init(dae::SceneObject& parent) override;
	virtual void Update(dae::SceneObject& parent) override;

private:
	std::weak_ptr<dae::SceneObject> m_QBertRef;


};


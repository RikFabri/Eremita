#pragma once
#include <BaseComponent.h>
#include <memory>
#include <vector>


class KillPlayerOnTouchComponent : public dae::BaseComponent
{
public:
	virtual void Init(dae::SceneObject& parent) override;
	virtual void Update(dae::SceneObject& parent) override;

private:
	std::vector<std::weak_ptr<dae::SceneObject>> m_QBertRefs;


};


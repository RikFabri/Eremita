#pragma once
#include <BaseComponent.h>
#include <memory>

class SlickAndSamBehaviourComponent final : public dae::BaseComponent
{
public:
	virtual void Init(dae::SceneObject& parent) override;
	virtual void Update(dae::SceneObject& parent) override;

private:
	void DieIfQbertClose(dae::SceneObject& parent);
	
	
	// No ownership
	std::weak_ptr<dae::SceneObject> m_QBertRef;


};


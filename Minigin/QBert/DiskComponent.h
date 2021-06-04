#pragma once
#include <BaseComponent.h>

namespace dae {
	class RenderComponent;
}


class DiskComponent : public dae::BaseComponent
{
public:
	virtual void Init(dae::SceneObject&) override;


private:


	// No ownership
	dae::RenderComponent* m_pRenderCompRef;
};


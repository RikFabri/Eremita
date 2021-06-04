#include "DiskComponent.h"
#include <SceneObject.h>
#include <RenderComponent.h>

void DiskComponent::Init(dae::SceneObject& parent)
{
	m_pRenderCompRef = parent.GetFirstComponentOfType<dae::RenderComponent>();

	if(!m_pRenderCompRef)
		throw std::exception("DiskComponent couldn't find sibling renderComponent");

	m_pRenderCompRef->SetTexture("Disk.png");
}

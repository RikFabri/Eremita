#include "MiniginPCH.h"
#include "SceneObject.h"

#include "BaseComponent.h"

dae::SceneObject::SceneObject(const std::vector<BaseComponent*> components)
	: m_Components(components)
{
	
}

dae::SceneObject::~SceneObject()
{
	for (auto* pComponent : m_Components)
		delete pComponent;
	
	for (auto* pComponent : m_GraphicalComponents)
		delete pComponent;
}


void dae::SceneObject::Update()
{
	for(auto* pComponent : m_Components)
		pComponent->FixedUpdate();
}

void dae::SceneObject::Render() const
{
	for (auto* pComponent : m_GraphicalComponents)
		pComponent->FixedUpdate();
}

void dae::SceneObject::AddComponent(BaseComponent* component, bool isGraphical)
{
	if (!isGraphical)
	{
		m_Components.push_back(component);
		return;
	}

	m_GraphicalComponents.push_back(component);
}

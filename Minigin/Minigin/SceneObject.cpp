#include "MiniginPCH.h"
#include "SceneObject.h"

#include "BaseComponent.h"

dae::SceneObject::SceneObject(const std::vector<BaseComponent*>& components, const glm::vec3& position)
	: m_Components(components)
{
	m_Components.push_back(new Transform(position));
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
	for (auto* pComponent : m_Components)
		pComponent->Update(*this);
}

void dae::SceneObject::FixedUpdate()
{
	for (auto* pComponent : m_Components)
		pComponent->FixedUpdate(*this);
}

void dae::SceneObject::Render()
{
	for (auto* pComponent : m_GraphicalComponents)
		pComponent->Update(*this);
}

void dae::SceneObject::Init()
{
	for (auto* pComponent : m_Components)
		pComponent->Init(*this);
	for (auto* pComponent : m_GraphicalComponents)
		pComponent->Init(*this);
}

void dae::SceneObject::AddComponent(BaseComponent* component, const bool isGraphical)
{
	if (!isGraphical)
	{
		m_Components.push_back(component);
		return;
	}

	m_GraphicalComponents.push_back(component);
}

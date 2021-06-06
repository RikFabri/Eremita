#include "MiniginPCH.h"
#include "SceneObject.h"

#include "SubjectComponent.h"
#include "BaseComponent.h"

const std::string& dae::SceneObject::GetTag() const
{
	return m_Tag;
}

dae::Scene* dae::SceneObject::GetScene() const
{
	return m_pScene;
}

const dae::Transform* dae::SceneObject::GetTransform() const
{
	return m_pTransform;
}


dae::SceneObject::SceneObject(const std::vector<BaseComponent*>& components, const glm::vec3& position, const glm::vec2& scale, const std::string& tag)
	: m_Components(components)
	, m_Tag(tag)
	, m_pScene(nullptr)
{
	m_pTransform = new Transform(position, scale);
	m_Components.push_back(m_pTransform);
}

dae::SceneObject::~SceneObject()
{
	for (auto* pComponent : m_Components)
		delete pComponent;
	
	for (auto* pComponent : m_GraphicalComponents)
		delete pComponent;


	// In case the user closes right after these queues got populated
	for (auto& pComponent : m_ComponentsToBeAdded)
		delete pComponent.first;

	for (auto& pComponent : m_ComponentsToBeRemoved)
		delete pComponent.first;
}

void dae::SceneObject::SetScene(Scene* pScene)
{
	m_pScene = pScene;
}

void dae::SceneObject::SetTag(const std::string& newTag)
{
	m_Tag = newTag;
}

void dae::SceneObject::SetPosition(const glm::vec3& pos)
{
	m_pTransform->SetPosition(pos.x, pos.y, pos.z);
}

void dae::SceneObject::SetPosition(float x, float y, float z)
{
	m_pTransform->SetPosition(x, y, z);
}

bool dae::SceneObject::RegisterAsObserver(ObserverInterface* pObserver)
{
	auto subjectComponent = GetFirstComponentOfType<SubjectComponent>();

	if (!subjectComponent)
		return false;

	subjectComponent->Subscribe(pObserver);

	return true;
}

void dae::SceneObject::UnRegisterObserver(ObserverInterface* pObserver)
{
	auto subjectComponent = GetFirstComponentOfType<SubjectComponent>();

	if(subjectComponent)
		subjectComponent->Unsubscribe(pObserver);
}

void dae::SceneObject::Update()
{
	for (auto* pComponent : m_Components)
		pComponent->Update(*this);

	// Add components created during update
	for (auto& comp : m_ComponentsToBeAdded)
	{
		// differentiate between normal and graphical components
		if (!comp.second)
		{
			m_Components.push_back(comp.first);
			continue;
		}
		m_GraphicalComponents.push_back(comp.first);
	}
	m_ComponentsToBeAdded.clear();

	// Remove components removed during update
	for (auto& comp : m_ComponentsToBeRemoved)
	{
		delete comp.first;
		// differentiate between normal and graphical components
		if (!comp.second)
		{
			m_Components.erase(std::find(m_Components.begin(), m_Components.end(), comp.first));
			continue;
		}
		m_GraphicalComponents.erase(std::find(m_GraphicalComponents.begin(), m_GraphicalComponents.end(), comp.first));
	}
	m_ComponentsToBeRemoved.clear();
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

void dae::SceneObject::AddComponentAfterUpdate(BaseComponent* component, bool isGraphical)
{
	m_ComponentsToBeAdded.push_back({ component, isGraphical });
}

void dae::SceneObject::RemoveComponent(BaseComponent* component, bool isGraphical)
{
	m_ComponentsToBeRemoved.push_back({ component, isGraphical });
}

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

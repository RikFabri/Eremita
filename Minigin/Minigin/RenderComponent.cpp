#include "MiniginPCH.h"
#include "RenderComponent.h"

#include "ResourceManager.h"
#include "Renderer.h"
#include "SceneObject.h"
#include "Texture2D.h"
#include "Transform.h"

dae::RenderComponent::RenderComponent(const std::string& texturePath)
	: m_pTexture(ResourceManager::GetInstance().LoadTexture(texturePath))
{
}

dae::RenderComponent::RenderComponent()
	: m_pTexture(nullptr)
{
}

void dae::RenderComponent::Receive(int message)
{
	std::cout << "message received: " << message << std::endl;
}

void dae::RenderComponent::Update(SceneObject&)
{
	const auto& transform = m_pTransformRef->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, transform.x, transform.y);
}

void dae::RenderComponent::Init(SceneObject& object)
{
	m_pTransformRef = object.GetFirstComponentOfType<Transform>();
	if (!m_pTransformRef)
		throw std::exception("SceneObject didn't have transform, did you remove it manually?");
}

void dae::RenderComponent::SetTexture(SDL_Texture* texture)
{
	m_pTexture = std::make_shared<Texture2D>(texture);
}

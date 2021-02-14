#include "MiniginPCH.h"
#include "RenderComponent.h"

#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"

dae::RenderComponent::RenderComponent(const std::string& texturePath, const glm::fvec3& localPosition)
	: m_pTexture(ResourceManager::GetInstance().LoadTexture(texturePath))
	, m_LocalPosition(localPosition)
{
}

dae::RenderComponent::RenderComponent(const glm::fvec3& localPosition)
	: m_pTexture(nullptr)
	, m_LocalPosition(localPosition)
{
}

void dae::RenderComponent::Receive(int message)
{
	std::cout << "message received: " << message << std::endl;
}

void dae::RenderComponent::Update(SceneObject&)
{
	Renderer::GetInstance().RenderTexture(*m_pTexture, m_LocalPosition.x, m_LocalPosition.y);
}

void dae::RenderComponent::SetTexture(SDL_Texture* texture)
{
	m_pTexture = std::make_shared<Texture2D>(texture);
}

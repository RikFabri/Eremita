#include "MiniginPCH.h"
#include "RenderComponent.h"

#include "ResourceManager.h"
#include "Renderer.h"
#include "SceneObject.h"
#include "Texture2D.h"
#include "Transform.h"

dae::RenderComponent::RenderComponent(const std::string& texturePath, const glm::vec2& posOffset)
	: m_pTexture(ResourceManager::GetInstance().LoadTexture(texturePath))
	, m_PositionOffset(posOffset)
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
	const auto& scale = m_pTransformRef->GetScale();
	const auto dimensions = m_pTexture->GetDimensions();

	Renderer::GetInstance().RenderTexture(*m_pTexture,
		transform.x + m_PositionOffset.x, transform.y + m_PositionOffset.y,
		dimensions.x * scale.x, dimensions.y * scale.y);
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

void dae::RenderComponent::SetTexture(std::shared_ptr<Texture2D> texture)
{
	m_pTexture = texture;
}

#include "TileComponent.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"

using namespace dae;

const int TileComponent::TileSize = 64;

TileComponent::TileComponent(glm::vec2 posOffset, int requiredJumps, bool resetJumps)
	: m_RequiredJumps(requiredJumps)
	, m_ResetOnSecondJump(resetJumps)
	, m_PositionOffset(posOffset)
	, m_CurrentJumps(0)
	, m_pParentTransformRef(nullptr)
{
}

void TileComponent::Init(SceneObject& parent)
{
	m_pParentTransformRef = parent.GetFirstComponentOfType<dae::Transform>();

	m_TileTex = ResourceManager::GetInstance().LoadTexture("Tile1.png");
}

void TileComponent::Update(SceneObject&)
{
	const auto pos = m_pParentTransformRef->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_TileTex, pos.x + m_PositionOffset.x, pos.y + m_PositionOffset.y, TileSize, TileSize);
}

int TileComponent::GetTileSize()
{
	return TileSize;
}
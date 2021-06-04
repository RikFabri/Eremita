#include "TileComponent.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"

using namespace dae;

const int TileComponent::TileSize = 64;

TileComponent::TileComponent(glm::vec2 posOffset, int requiredJumps, bool resetJumps)
	: m_RequiredJumps(resetJumps ? 1 : requiredJumps) // You can never reach more than one jump when reset is turned on
	, m_ResetOnSecondJump(resetJumps)
	, m_PositionOffset(posOffset)
	, m_CurrentJumps(0)
	, m_pParentTransformRef(nullptr)
{
}

void TileComponent::Init(SceneObject& parent)
{
	m_pParentTransformRef = parent.GetFirstComponentOfType<dae::Transform>();

	m_TileTex = ResourceManager::GetInstance().LoadTexture("Tile1_0.png");
}

void TileComponent::Update(SceneObject&)
{
	const auto pos = m_pParentTransformRef->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_TileTex, pos.x + m_PositionOffset.x, pos.y + m_PositionOffset.y, TileSize, TileSize);
}

TileComponent::TileState TileComponent::HopOnTile()
{
	if (m_ResetOnSecondJump)
	{
		if (m_CurrentJumps >= 1)
		{
			m_CurrentJumps = 0;
			m_TileTex = ResourceManager::GetInstance().LoadTexture("Tile1_" + std::to_string(m_CurrentJumps) + ".png");
			return TileState::eUncompleted;
		}
	}

	if (m_CurrentJumps >= m_RequiredJumps)
		return TileState::eAlreadyCompleted;


	++m_CurrentJumps;

	m_TileTex = ResourceManager::GetInstance().LoadTexture("Tile1_" + std::to_string(m_CurrentJumps) + ".png");
	if (m_CurrentJumps >= m_RequiredJumps)
	{
		return TileState::eNewlyCompleted;
	}
	return TileState::eProgress;
}

int TileComponent::GetTileSize()
{
	return TileSize;
}
#include "PosessedMovementComponent.h"
#include "TileMapComponent.h"
#include "TimerComponent.h"
#include "Transform.h"
#include "Scene.h"

void PosessedMovementComponent::Init(dae::SceneObject& parent)
{
	auto tileMapObj = parent.GetScene()->GetObjectsByTag("tileMap");
	m_pTileMapRef = tileMapObj[0]->GetFirstComponentOfType<TileMapComponent>();

	m_pTimerCompRef = parent.GetFirstComponentOfType<dae::TimerComponent>();
	m_pTransformRef = parent.GetFirstComponentOfType<dae::Transform>();
}

void PosessedMovementComponent::Update(dae::SceneObject&)
{
	if (m_UsedDisk)
	{
		m_UsedDisk = false;
		m_PrevPos = m_pTransformRef->GetPosition();
	}
}

void PosessedMovementComponent::Move(int x, int y)
{
	if (!m_pTimerCompRef->TimerCompleted())
	{
		return;
	}

	m_pTimerCompRef->Reset();

	m_Index.first += x;
	m_Index.second += y;

	m_PrevPos = m_pTransformRef->GetPosition();
	m_UsedDisk = false;
	auto isDisk = false;
	auto isValid = m_pTileMapRef->IsBlockIndexValid(m_Index);

	if (!isValid)
		isDisk = isValid = m_pTileMapRef->IsBlockIndexDisk(m_Index);

	if (isValid)
	{
		if (!isDisk)
			m_pTileMapRef->HoppedOnTile(m_Index);
		else
		{
			m_pTileMapRef->HoppedOnDisk(m_Index, m_pParent);
			m_Index = { 0, 0 };
			m_UsedDisk = true;
		}
	}
	else
	{
		if (m_JumpedOff)
			m_JumpedOff();
	}

	const auto pos = m_pTileMapRef->IndexToTilePosition(m_Index);
	m_pTransformRef->SetPosition(pos.x, pos.y, 0);

	if (m_UsedDisk)
	{
		m_PrevPos.x = pos.x;
		m_PrevPos.y = pos.y;
	}
}

void PosessedMovementComponent::SetBlockIndex(const int2& idx)
{
	m_Index = idx;
}

void PosessedMovementComponent::UpdateVisualLocation()
{
	const auto pos = m_pTileMapRef->IndexToTilePosition(m_Index);
	m_pTransformRef->SetPosition(pos.x, pos.y, 0);
}

bool PosessedMovementComponent::UsedDisk() const
{
	return m_UsedDisk;
}

const glm::vec3& PosessedMovementComponent::GetPreviousPos() const
{
	return m_PrevPos;
}

void PosessedMovementComponent::SetJumpedOffCallback(const std::function<void()>& func)
{
	m_JumpedOff = func;
}
#include "PosessedMovementComponent.h"
#include "SoundServiceLocator.h"
#include "TileMapComponent.h"
#include "TimerComponent.h"
#include "ScoreComponent.h"
#include "Transform.h"
#include "Scene.h"
#include "SDL.h"

PosessedMovementComponent::PosessedMovementComponent(bool canInteractWBlocks, bool canUseDisks, bool canJumpOff, bool isEnabled)
	: m_CanJumpOff(canJumpOff)
	, m_CanUseDisks(canUseDisks)
	, m_InteractWithBlocks(canInteractWBlocks)
	, m_Enabled(isEnabled)
{
}

void PosessedMovementComponent::Init(dae::SceneObject& parent)
{
	auto tileMapObj = parent.GetScene()->GetObjectsByTag("tileMap");
	m_pTileMapRef = tileMapObj[0]->GetFirstComponentOfType<TileMapComponent>();

	m_pTimerCompRef = parent.GetFirstComponentOfType<dae::TimerComponent>();
	m_pTransformRef = parent.GetFirstComponentOfType<dae::Transform>();
	m_pParent = &parent;

	m_pScoreCompRef = parent.GetFirstComponentOfType<dae::ScoreComponent>();
}

void PosessedMovementComponent::Update(dae::SceneObject&)
{
	if (!m_Enabled)
		return;
}

void PosessedMovementComponent::Move(int x, int y)
{
	if (!m_Enabled)
		return;

	if (!m_pTimerCompRef->TimerCompleted())
	{
		return;
	}

	m_pTimerCompRef->Reset();

	// Play sound
	// Quick solution to not knowing which sound to play, I know, not very clean...
	const std::string soundFileName = m_CanJumpOff ? "QbertJmp.wav" : "CoilyJmp.wav";
	auto* const service = dae::SoundServiceLocator::GetSoundService();
	service->PlaySound("../Data/" + soundFileName, SDL_MIX_MAXVOLUME / 2);

	m_Index.first += x;
	m_Index.second += y;

	m_PrevPos = m_pTransformRef->GetPosition();
	auto isDisk = false;
	auto isValid = m_pTileMapRef->IsBlockIndexValid(m_Index);

	if (!isValid && m_CanUseDisks)
		isDisk = isValid = m_pTileMapRef->IsBlockIndexDisk(m_Index);

	if (isValid)
	{
		if (!isDisk)
		{
			if (m_InteractWithBlocks)
			{
				const auto causedColourChange = m_pTileMapRef->HoppedOnTile(m_Index);
				if (causedColourChange && m_pScoreCompRef)
					m_pScoreCompRef->IncreaseScore(25);
			}
		}
		else
		{
			m_pTileMapRef->HoppedOnDisk(m_Index, m_pParent);
			m_Index = { 0, 0 };
			m_OnDisk = true;
		}
	}
	else
	{
		if (m_CanJumpOff && m_JumpedOff)
			m_JumpedOff();
		else
		{
			m_Index.first -= x;
			m_Index.second -= y;
			return;
		}
	}

	const auto pos = m_pTileMapRef->IndexToTilePosition(m_Index);
	m_pTransformRef->SetPosition(pos.x, pos.y, 0);
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

void PosessedMovementComponent::SetEnabled(bool enabled)
{
	m_Enabled = enabled;
}

bool PosessedMovementComponent::IsOnDisk() const
{
	return m_OnDisk;
}

void PosessedMovementComponent::SetOnDisk(bool isOnDisk)
{
	m_OnDisk = isOnDisk;
}

void PosessedMovementComponent::SetPreviousPos(const glm::vec3& pos)
{
	m_PrevPos = pos;
}

const glm::vec3& PosessedMovementComponent::GetPreviousPos() const
{
	return m_PrevPos;
}

void PosessedMovementComponent::SetJumpedOffCallback(const std::function<void()>& func)
{
	m_JumpedOff = func;
}

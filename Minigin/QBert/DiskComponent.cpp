#include "DiskComponent.h"
#include "PosessedMovementComponent.h"
#include <SceneObject.h>
#include <RenderComponent.h>
#include "TileMapComponent.h"
#include "ScoreComponent.h"
#include "Renderer.h"
#include "GameTime.h"
#include "Scene.h"

void DiskComponent::Init(dae::SceneObject& parent)
{
	m_Moving = false;

	m_pRenderCompRef = parent.GetFirstComponentOfType<dae::RenderComponent>();

	if(!m_pRenderCompRef)
		throw std::exception("DiskComponent couldn't find sibling renderComponent");

	m_pRenderCompRef->SetTexture("Disk.png");

	// Set all positions for the bezier curve
	const auto pos = parent.GetTransform()->GetPosition();
	const auto dimensions = dae::Renderer::GetInstance().GetWindowDimensions();

	if (dimensions.first / 2 > pos.x)
	{
		m_BezierControlPoint = { 0, 0 };
	}
	else
	{
		m_BezierControlPoint = { dimensions.first, 0 };
	}
	m_Percentage = 0;
	m_StartPos = pos;
}

void DiskComponent::Update(dae::SceneObject& parent)
{
	if (!m_Moving)
		return;

	m_Percentage += (float)dae::GameTime::GetInstance().GetDeltaTime() / 2.8f;

	const auto pos = glm::vec3{ GetPointOnBezier(m_Percentage), 0 };
	parent.SetPosition(pos - glm::vec3{ 0, 16, 0 });
	m_pQbertRef->SetPosition(pos);

	// Arrived at destination
	if (m_Percentage >= 1)
	{
		m_Moving = false;
		auto movement = m_pQbertRef->GetFirstComponentOfType<PosessedMovementComponent>();

		if (movement)
		{
			movement->SetEnabled(true);
			movement->SetBlockIndex({ 0, 0 });
			movement->UpdateVisualLocation();
			movement->SetOnDisk(false);
			movement->SetPreviousPos(m_pQbertRef->GetTransform()->GetPosition());

			auto tileMap = parent.GetScene()->GetObjectsByTag("tileMap");
			if (!tileMap.empty())
			{
				auto tileMapComp = tileMap[0]->GetFirstComponentOfType<TileMapComponent>();
				if (tileMapComp)
				{
					if (tileMapComp->HoppedOnTile({ 0, 0 }))
					{
						auto score = m_pQbertRef->GetFirstComponentOfType<dae::ScoreComponent>();
						if (score)
							score->IncreaseScore(25);
					}
				}
			}
		}
		parent.GetScene()->Remove(&parent);
	}
}

void DiskComponent::MoveToSpawn(dae::SceneObject* qbert, const glm::vec2& targetPos)
{
	m_Moving = true;
	m_pQbertRef = qbert;
	m_TargetPos = targetPos;
}

glm::vec2 DiskComponent::GetPointOnBezier(float percentage) const
{
	// Control + (1 - t)² (start - control) + t² (end - control)
	return m_BezierControlPoint + (1 - percentage) * (1 - percentage) * (m_StartPos - m_BezierControlPoint) + percentage * percentage * (m_TargetPos - m_BezierControlPoint);
}

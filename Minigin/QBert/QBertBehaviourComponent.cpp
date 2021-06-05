#include "QBertBehaviourComponent.h"
#include "RenderComponent.h"
#include "TileMapComponent.h"
#include "HealthComponent.h"
#include "TimerComponent.h"
#include "Scene.h"

void QBertBehaviourComponent::Init(dae::SceneObject& parent)
{
	m_UsedDisk = false;

	m_pParent = &parent;

	const auto renderComp = new dae::RenderComponent("QBert.png", {16, -16});
	renderComp->Init(parent);
	parent.AddComponent(renderComp, true);

	auto tileMapObj = parent.GetScene()->GetObjectsByTag("tileMap");

	m_pTimerCompRef = new dae::TimerComponent(0.5f);
	m_pTimerCompRef->Init(parent);
	parent.AddComponent(m_pTimerCompRef);

	if (tileMapObj.empty())
		throw std::exception("QBertBehaviour couldn't find a tileMap in the level, did you forget to add it?");

	m_pTileMapRef = tileMapObj[0]->GetFirstComponentOfType<TileMapComponent>();

	if(!m_pTileMapRef)
		throw std::exception("QBertBehaviour::init > tileMap didn't have a tileMapComponent");

	m_pTransformRef = parent.GetFirstComponentOfType<dae::Transform>();
	m_pHealthCompRef = parent.GetFirstComponentOfType<dae::HealthComponent>();

	const auto pos = m_pTileMapRef->IndexToTilePosition({ 0, 0 });
	m_pTransformRef->SetPosition(pos.x, pos.y, 0);
}

void QBertBehaviourComponent::Update(dae::SceneObject&)
{
	if (m_pTimerCompRef->TimerCompleted())
	{
		if (m_UsedDisk)
		{
			m_UsedDisk = false;
			m_PrevPos = m_pTransformRef->GetPosition();
		}
	}
}

void QBertBehaviourComponent::Move(int x, int y)
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
		Damage();
	}

	const auto pos = m_pTileMapRef->IndexToTilePosition(m_Index);
	m_pTransformRef->SetPosition(pos.x, pos.y, 0);

	if (m_UsedDisk)
	{
		m_PrevPos.x = pos.x;
		m_PrevPos.y = pos.y;
	}
}

void QBertBehaviourComponent::Damage()
{
	m_pHealthCompRef->Die();

	m_Index = { 0, 0 };
	const auto pos = m_pTileMapRef->IndexToTilePosition(m_Index);
	m_pTransformRef->SetPosition(pos.x, pos.y, 0);

}

bool QBertBehaviourComponent::UsedDisk() const
{
	return m_UsedDisk;
}

const glm::vec3& QBertBehaviourComponent::GetPreviousPos() const
{
	return m_PrevPos;
}

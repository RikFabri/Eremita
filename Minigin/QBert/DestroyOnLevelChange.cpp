#include "DestroyOnLevelChange.h"
#include "TileMapComponent.h"
#include "SceneObject.h"
#include "Scene.h"

DestroyOnLevelChange::~DestroyOnLevelChange()
{
	if (m_pTileMapRef.expired())
		return;

	m_pTileMapRef.lock()->UnRegisterObserver(this);
}

void DestroyOnLevelChange::Init(dae::SceneObject& parent)
{
	m_pParentRef = &parent;

	m_pTileMapRef = parent.GetScene()->GetObjectsByTag("tileMap")[0];
	m_pTileMapRef.lock()->RegisterAsObserver(this);
}

void DestroyOnLevelChange::OnNotify(const BaseComponent*, const std::string& message)
{
	if(message == "level Changed")
		m_pParentRef->GetScene()->Remove(m_pParentRef);
}

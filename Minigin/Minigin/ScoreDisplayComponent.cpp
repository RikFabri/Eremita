#include "ScoreDisplayComponent.h"

#include "Scene.h"
#include "ScoreComponent.h"
#include "SceneObject.h"
#include "SubjectComponent.h"
#include "TextComponent.h"

dae::ScoreDisplayComponent::ScoreDisplayComponent(int playerIndex)
	: m_PlayerIndex(playerIndex)
{
}

void dae::ScoreDisplayComponent::Init(SceneObject& parent)
{
	m_pTextComponentRef = parent.GetFirstComponentOfType<TextComponent>();

	if (!m_pTextComponentRef)
		throw std::exception("ScoreDisplayComponent couldn't find a sibling textComponent");

	const auto playerObjects = parent.GetScene()->GetObjectsByTag("player");
	
	if ((size_t)m_PlayerIndex >= playerObjects.size())
		throw std::exception("Couldn't find player with id: " + m_PlayerIndex);
	
	const auto wasSuccess = playerObjects[m_PlayerIndex]->RegisterAsObserver(this);
	if (!wasSuccess)
		throw std::exception("ScoreDisplayComponent couldn't register as observer, does qbert have a subjectComponent?");
}

void dae::ScoreDisplayComponent::OnNotify(const BaseComponent* sender, const std::string& message)
{
	if (message != "UpdateScore")
		return;

	const auto* scoreComp = static_cast<const ScoreComponent*>(sender);
	
	const auto score = "Score: " + std::to_string(scoreComp->GetScore());
	m_pTextComponentRef->SetText(score);
}

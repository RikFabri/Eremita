#include "ScoreDisplayComponent.h"

#include "Scene.h"
#include "ScoreComponent.h"
#include "SceneObject.h"
#include "SubjectComponent.h"
#include "TextComponent.h"

void dae::ScoreDisplayComponent::Init(SceneObject& parent)
{
	m_pTextComponentRef = parent.GetFirstComponentOfType<TextComponent>();

	if (!m_pTextComponentRef)
		throw std::exception("ScoreDisplayComponent couldn't find a sibling textComponent");

	const auto wasSuccess = parent.GetScene()->GetObjectsByTag("player")[0]->RegisterAsObserver(this);
	if (!wasSuccess)
		throw std::exception("ScoreDisplayComponent couldn't register as observer, does qbert have a subjectComponent?");
}

void dae::ScoreDisplayComponent::OnNotify(const BaseComponent* sender, const std::string& message)
{
	if (message != "ScoreIncreased")
		return;

	const auto* scoreComp = static_cast<const ScoreComponent*>(sender);
	
	const auto score = "Score: " + std::to_string(scoreComp->GetScore());
	m_pTextComponentRef->SetText(score);
}

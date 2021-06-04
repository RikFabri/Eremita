#pragma once
#include <BaseComponent.h>
#include <TimerComponent.h>
#include <memory>

namespace dae
{
	class RenderComponent;
	class TimerComponent;
}

class TileMapComponent;
class DefaultMovement;

using int2 = std::pair<int, int>;

class CoilyBehaviourComponent final : public dae::BaseComponent
{
public:
	CoilyBehaviourComponent();

	virtual void Init(dae::SceneObject&) override;

	virtual void Update(dae::SceneObject&) override;

private:
	void KillQbertIfClose(dae::SceneObject& parent);
	void HatchEgg(dae::SceneObject& parent);
	void UpdateCoily(dae::SceneObject& parent);

	bool m_IsEgg;
	// Positional index on the map
	int2 m_Index;

	//No ownership
	std::weak_ptr<dae::SceneObject> m_QBertRef;
	dae::TimerComponent* m_pTimerCompRef;
	TileMapComponent* m_pTileMapRef;
	dae::RenderComponent* m_pRenderCompRef;
	BaseComponent* m_pDefaultMovement;
};


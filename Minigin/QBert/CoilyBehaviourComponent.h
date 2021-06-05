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
	CoilyBehaviourComponent(bool isPossessed = false);

	virtual void Init(dae::SceneObject&) override;

private:
	void HatchEgg(dae::SceneObject& parent);

	// Whether or not to use ai/player input after hatching
	bool m_Possessed;
	// Positional index on the map
	int2 m_Index;

	//No ownership
	dae::TimerComponent* m_pTimerCompRef;
	TileMapComponent* m_pTileMapRef;
	dae::RenderComponent* m_pRenderCompRef;
	BaseComponent* m_pDefaultMovement;
};


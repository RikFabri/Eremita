#pragma once
#include <BaseComponent.h>
#include <utility>

namespace dae
{
	class TimerComponent;
}

class TileMapComponent;
using int2 = std::pair<int, int>;

class WrongwayMovementComponent : public dae::BaseComponent
{
public:
	virtual void Init(dae::SceneObject& parent) override;
	virtual void Update(dae::SceneObject& parent) override;

private:
	int2 m_Index;

	// No ownership
	TileMapComponent* m_pTileMapRef;
	dae::TimerComponent* m_pTimerCompRef;
};


#pragma once
#include <BaseComponent.h>
#include <utility>
#include <memory>

namespace dae {
	class TimerComponent;
}

using int2 = std::pair<int, int>;
class TileMapComponent;
class PosessedMovementComponent;

class CoilyMovementComponent : public dae::BaseComponent
{
public:
	virtual void Init(dae::SceneObject& parent) override;
	virtual void Update(dae::SceneObject& parent) override;

	void SetIndex(const int2& idx);
private:
	int2 m_Index;

	// No ownership
	std::weak_ptr<dae::SceneObject> m_QBertRef;
	PosessedMovementComponent* m_pQBertMovementCompRef;
	TileMapComponent* m_pTileMapRef;
	dae::TimerComponent* m_pTimerCompRef;

};


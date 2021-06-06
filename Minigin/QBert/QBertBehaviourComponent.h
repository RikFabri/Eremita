#pragma once
#include <BaseComponent.h>
#include <SceneObject.h>
#include "ObserverInterface.h"

namespace dae 
{
	class HealthComponent;
	class TimerComponent;
}

using int2 = std::pair<int, int>;
class TileMapComponent;
class PosessedMovementComponent;

class QBertBehaviourComponent final : public dae::BaseComponent, public dae::ObserverInterface
{
public:
	virtual void Init(dae::SceneObject&) override;
	virtual void Update(dae::SceneObject&) override;

	virtual void OnNotify(const BaseComponent*, const std::string& message) override;


	void Damage();
private:
	float m_PrevWaitTime;
	bool m_Alive;

	// No ownership
	PosessedMovementComponent* m_pMovementCompRef;
	TileMapComponent* m_pTileMapRef;
	dae::Transform* m_pTransformRef;
	dae::HealthComponent* m_pHealthCompRef;
	dae::TimerComponent* m_pTimerCompRef;
	dae::SceneObject* m_pParent;
};


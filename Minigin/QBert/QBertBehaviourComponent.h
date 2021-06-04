#pragma once
#include <BaseComponent.h>
#include <SceneObject.h>


namespace dae 
{
	class HealthComponent;
	class TimerComponent;
}

class TileMapComponent;

class QBertBehaviourComponent : public dae::BaseComponent
{
public:
	virtual void Init(dae::SceneObject&) override;

	virtual void Update(dae::SceneObject&) override;

	void Move(int x, int y);
private:
	int m_X, m_Y;



	// No ownership
	TileMapComponent* m_pTileMapRef;
	dae::Transform* m_pTransformRef;
	dae::HealthComponent* m_pHealthCompRef;
	dae::TimerComponent* m_pTimerCompRef;
};


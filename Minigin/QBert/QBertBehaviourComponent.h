#pragma once
#include <BaseComponent.h>
#include <SceneObject.h>


namespace dae 
{
	class HealthComponent;
	class TimerComponent;
}

using int2 = std::pair<int, int>;
class TileMapComponent;


class QBertBehaviourComponent final : public dae::BaseComponent
{
public:
	virtual void Init(dae::SceneObject&) override;

	virtual void Update(dae::SceneObject&) override;

	void Move(int x, int y);
private:
	int2 m_Index;

	// No ownership
	TileMapComponent* m_pTileMapRef;
	dae::Transform* m_pTransformRef;
	dae::HealthComponent* m_pHealthCompRef;
	dae::TimerComponent* m_pTimerCompRef;
	dae::SceneObject* m_pParent;
};


#pragma once
#include <BaseComponent.h>
#include <memory>

namespace dae
{
	class TimerComponent;
	class Scene;
}

class EnemySpawner : public dae::BaseComponent
{
public:
	EnemySpawner(bool isCoilyPlayer = false);

	virtual void Init(dae::SceneObject& parent) override;
	virtual void Update(dae::SceneObject&) override;

private:
	bool m_CoilyIsPlayer;

	// No ownership
	dae::TimerComponent* m_pTimerCompRef;
	std::weak_ptr<dae::Scene> m_pSceneRef;
	std::weak_ptr<dae::SceneObject> m_pCoilyRef;

	void SpawnCoily(bool posessed = false);
	void SpawnUggOrWrongway();
	void SpawnSlickOrSam();
	void SpawnWrongway();
	void SpawnUgg();
};


#pragma once
#include <BaseComponent.h>
#include <utility>
#include <functional>

namespace dae
{
	class TimerComponent;
}

using int2 = std::pair<int, int>;
class TileMapComponent;

class DefaultMovement : public dae::BaseComponent
{
public:
	DefaultMovement(bool moveUp = false);

	virtual void Init(dae::SceneObject& parent) override;
	virtual void Update(dae::SceneObject& parent) override;

	void SetReachedEnd(std::function<void(dae::SceneObject& parent)> callback);
private:
	std::function<void(dae::SceneObject& parent)> m_ReachedEndCallback;

	bool m_MoveUp;
	int2 m_Index;

	// No ownership
	TileMapComponent* m_pTileMapRef;
	dae::TimerComponent* m_pTimerCompRef;
};


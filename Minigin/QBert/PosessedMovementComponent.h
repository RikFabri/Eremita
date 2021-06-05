#pragma once
#include <BaseComponent.h>
#include <SceneObject.h>
#include <functional>
#include <utility>

namespace dae
{
	class TimerComponent;
	class Transform;
}
class TileMapComponent;

using int2 = std::pair<int, int>;

class PosessedMovementComponent : public dae::BaseComponent
{
public:
	virtual void Init(dae::SceneObject& parent) override;
	virtual void Update(dae::SceneObject& parent) override;

	void Move(int x, int y);

	// Sets the block index but doesn't automatically update position
	void SetBlockIndex(const int2& idx);
	void UpdateVisualLocation();
	bool UsedDisk() const;
	const glm::vec3& GetPreviousPos() const;
	void SetJumpedOffCallback(const std::function<void()>& func);
private:
	std::function<void()> m_JumpedOff;
	int2 m_Index;
	bool m_UsedDisk;
	glm::vec3 m_PrevPos;

	// No ownership
	dae::TimerComponent* m_pTimerCompRef;
	dae::Transform* m_pTransformRef;
	TileMapComponent* m_pTileMapRef;
	dae::SceneObject* m_pParent;


};


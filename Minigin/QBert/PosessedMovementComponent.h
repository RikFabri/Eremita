#pragma once
#include <BaseComponent.h>
#include <SceneObject.h>
#include <functional>
#include <utility>

namespace dae
{
	class TimerComponent;
	class Transform;
	class ScoreComponent;
}
class TileMapComponent;

using int2 = std::pair<int, int>;

class PosessedMovementComponent : public dae::BaseComponent
{
public:
	PosessedMovementComponent(bool canInteractWBlocks = true, bool canUseDisks = true, bool canJumpOff = true, bool isEnabled = true);

	virtual void Init(dae::SceneObject& parent) override;
	virtual void Update(dae::SceneObject& parent) override;

	void Move(int x, int y);

	// Sets the block index but doesn't automatically update position
	void SetBlockIndex(const int2& idx);
	void UpdateVisualLocation();
	void SetEnabled(bool enabled);
	bool IsOnDisk() const;
	void SetOnDisk(bool isOnDisk);
	void SetPreviousPos(const glm::vec3& pos);
	const glm::vec3& GetPreviousPos() const;
	const int2& GetBlockIndex() const;
	void SetJumpedOffCallback(const std::function<void()>& func);
private:
	bool m_Enabled;
	bool m_InteractWithBlocks;
	bool m_CanUseDisks;
	bool m_CanJumpOff;

	std::function<void()> m_JumpedOff;
	int2 m_Index;
	bool m_OnDisk;
	glm::vec3 m_PrevPos;

	// No ownership
	dae::TimerComponent* m_pTimerCompRef;
	dae::ScoreComponent* m_pScoreCompRef;
	dae::Transform* m_pTransformRef;
	TileMapComponent* m_pTileMapRef;
	dae::SceneObject* m_pParent;


};


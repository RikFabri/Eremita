#pragma once
#include <BaseComponent.h>
#include <SceneObject.h>
#include <memory>

namespace dae {
	class Texture2D;
}

class TileComponent : public dae::BaseComponent
{
public:
	enum class TileState
	{
		// Tile was completed but got reset
		eUncompleted,
		// Tile isn't completed yet
		eProgress,
		// Tile just got completed
		eNewlyCompleted,
		// Tile was and still is complete
		eAlreadyCompleted
	};

	explicit TileComponent(glm::vec2 posOffset = { 0, 0 }, int requiredJumps = 2, bool resetJumps = true);
	virtual void Init(dae::SceneObject&) override;

	virtual void Update(dae::SceneObject&) override;
	
	// Updates the tileState and returns whether or not it's complete.
	TileState HopOnTile();

	static int GetTileSize();
private:
	// The amount of times this block needs to be jumped on
	const int m_RequiredJumps;
	// Whether or not this block resets on a second jump
	const bool m_ResetOnSecondJump;

	// The amount of times this block has been jumped on
	int m_CurrentJumps;
	
	std::shared_ptr<dae::Texture2D> m_TileTex;
	glm::vec2 m_PositionOffset;

	const static int TileSize;


	// No ownership
	dae::Transform* m_pParentTransformRef;
};


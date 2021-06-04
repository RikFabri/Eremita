#pragma once
#include <BaseComponent.h>
#include <SceneObject.h>
#include <memory>

class TileComponent;

class TileMapComponent : public dae::BaseComponent
{
public:
	virtual void Init(dae::SceneObject&) override;
	virtual void Update(dae::SceneObject&) override;

	void HoppedOnTile(const glm::vec2& blockIndex);

	bool IsBlockIndexValid(const glm::vec2& blockIndex) const;
	glm::vec2 IndexToTilePosition(const glm::vec2& cartesianCoords, bool absolute = true) const;
private:
	int m_RequiredNrOfTilesToComplete;
	int m_CompletedTiles;

	std::vector<std::vector<std::unique_ptr<TileComponent>>> m_Tiles;

	// No ownership
	dae::Transform* m_pTransformRef;
};


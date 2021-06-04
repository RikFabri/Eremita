#pragma once
#include <BaseComponent.h>
#include <SceneObject.h>
#include <memory>
#include <unordered_map>
#include <functional>

using int2 = std::pair<int, int>;

class TileComponent;
class QBertBehaviourComponent;

class TileMapComponent : public dae::BaseComponent
{
public:
	TileMapComponent();

	virtual void Init(dae::SceneObject&) override;
	virtual void Update(dae::SceneObject&) override;

	void HoppedOnTile(const int2& blockIndex);
	void HoppedOnDisk(const int2& blockIndex, dae::SceneObject* qBert);

	bool IsBlockIndexValid(const int2& blockIndex) const;
	bool IsBlockIndexDisk(const int2& blockIndex) const;
	glm::vec2 IndexToTilePosition(const int2& blockIndex, bool absolute = true) const;
private:
	int m_RequiredNrOfTilesToComplete;
	int m_CompletedTiles;

	std::vector<std::vector<std::unique_ptr<TileComponent>>> m_Tiles;
	std::unordered_map<int2, std::weak_ptr<dae::SceneObject>, 
		std::function<size_t(const int2&)>, std::function<bool(const int2&, const int2&)>> m_Disks;

	// No ownership
	dae::Transform* m_pTransformRef;
};

std::size_t int2hash(const int2& x);
bool compareInt2(const int2& lhs, const int2& rhs);


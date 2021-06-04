#include "TileManagerComponent.h"

#include "TileComponent.h"


void TileMapComponent::Init(dae::SceneObject& parent)
{
	const int maxTileLength = 7;
	// Offsets for fake perspective
	const int tileSize = TileComponent::GetTileSize();
	const int horizontalOffset =  tileSize / 2;
	const int verticalOffset = tileSize * 3 / 4;
	
	m_Tiles.reserve(maxTileLength);

	for(int i = 0; i < maxTileLength; ++i)
	{
		m_Tiles.push_back(std::vector<std::unique_ptr<TileComponent>>());
		for (int j = 0; j < i; ++j)
		{
			m_Tiles[i].push_back(std::make_unique<TileComponent>(glm::vec2{j * tileSize - i * horizontalOffset, i * verticalOffset}));
		}
	}

	for (auto& tileArray : m_Tiles)
		for(auto& tile : tileArray)
			tile->Init(parent);
}

void TileMapComponent::Update(dae::SceneObject& parent)
{
	for (auto& tileArray : m_Tiles)
		for (auto& tile : tileArray)
			tile->Update(parent);
}

glm::vec2 TileMapComponent::CartesianToAxial(const glm::vec2& cartesianCoords) const
{
	return glm::vec2();
}

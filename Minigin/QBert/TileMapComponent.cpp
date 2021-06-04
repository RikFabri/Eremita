#include "TileMapComponent.h"

#include "TileComponent.h"
#include <Logger.h>


void TileMapComponent::Init(dae::SceneObject& parent)
{
	// Create all tiles for the level
	const int maxNrOfTiles = 7;
	
	m_Tiles.reserve(maxNrOfTiles);

	for(int i = 0; i < maxNrOfTiles; ++i)
	{
		m_Tiles.push_back(std::vector<std::unique_ptr<TileComponent>>());
		for (int j = 0; j <= i; ++j)
		{
			m_Tiles[i].push_back(std::make_unique<TileComponent>(IndexToTilePosition({j, i}, false)));
			++m_RequiredNrOfTilesToComplete;
		}
	}

	// Initialize all tiles under the parent gameObject of the tileMap
	for (auto& tileArray : m_Tiles)
		for(auto& tile : tileArray)
			tile->Init(parent);

	m_pTransformRef = parent.GetFirstComponentOfType<dae::Transform>();
}

void TileMapComponent::Update(dae::SceneObject& parent)
{
	for (auto& tileArray : m_Tiles)
		for (auto& tile : tileArray)
			tile->Update(parent);
}

void TileMapComponent::HoppedOnTile(const glm::vec2& blockIndex)
{
	const auto tileChange = m_Tiles[(int)blockIndex.y][(int)blockIndex.x]->HopOnTile();

	switch (tileChange)
	{
	case TileComponent::TileState::eNewlyCompleted:
		++m_CompletedTiles;
		break;
	case TileComponent::TileState::eUncompleted:
		--m_CompletedTiles;
		break;
	default:
		break;
	}

	if (m_CompletedTiles >= m_RequiredNrOfTilesToComplete)
		dae::Logger::GetInstance().Print("Level complete");
}

bool TileMapComponent::IsBlockIndexValid(const glm::vec2& blockIndex) const
{
	const auto x = (int)blockIndex.x;
	const auto y = (int)blockIndex.y;

	if (y < 0 || y >= m_Tiles.size())
		return false;

	if (x < 0 || x >= m_Tiles[y].size())
		return false;

	if (m_Tiles[y][x] == nullptr)
		return false;

	return true;
}

glm::vec2 TileMapComponent::IndexToTilePosition(const glm::vec2& cartesianCoords, bool absolutePosition) const
{
	// Offsets for fake perspective
	const int tileSize = TileComponent::GetTileSize();
	const int horizontalOffset = tileSize / 2;
	const int verticalOffset = tileSize * 3 / 4;

	auto result =	glm::vec2(cartesianCoords.x * tileSize - cartesianCoords.y * horizontalOffset, cartesianCoords.y * verticalOffset);

	if (absolutePosition)
	{
		const auto pos = m_pTransformRef->GetPosition();
		result.x += pos.x;
		result.y += pos.y;
	}

	//dae::Logger::GetInstance().Print(std::to_string(cartesianCoords.x) + " | " + std::to_string(cartesianCoords.y));
	//dae::Logger::GetInstance().Print(std::to_string(result.x) + " | " + std::to_string(result.y));
	//dae::Logger::GetInstance().Print(R"(----------------------------------------------------------)");

	return result;
}

#include "TileMapComponent.h"
#include "DiskComponent.h"
#include "RenderComponent.h"
#include "TileComponent.h"
#include "QBertBehaviourComponent.h"
#include "ResourceManager.h"
#include "Logger.h"
#include <Scene.h>
#include <Logger.h>
#include <fstream>
#include <rapidjson.h>
#include <istreamwrapper.h>
#include <document.h>

TileMapComponent::TileMapComponent()
	: m_Disks(14, int2hash, compareInt2)
{
}

void TileMapComponent::Init(dae::SceneObject& parent)
{
	// Initialize this object
	m_pTransformRef = parent.GetFirstComponentOfType<dae::Transform>();


	LoadLevelFromFile("Level1.json", parent);

	// Initialize all tiles under the parent gameObject of the tileMap
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

void TileMapComponent::HoppedOnTile(const int2& blockIndex, bool forceReverse)
{
	auto tileChange = TileComponent::TileState::eProgress;

	if(!forceReverse)
		tileChange = m_Tiles[blockIndex.second][blockIndex.first]->HopOnTile();
	else
		tileChange = m_Tiles[blockIndex.second][blockIndex.first]->RevertTile();

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

void TileMapComponent::HoppedOnDisk(const int2& blockIndex, dae::SceneObject* qBert)
{
	const auto diskIt = m_Disks.find(blockIndex);
	const auto diskPos = IndexToTilePosition({ -1, -2 });

	if (!diskIt->second.expired())
	{
		const auto diskPtr = diskIt->second.lock();
		qBert->GetScene()->Remove(diskPtr);
		diskPtr->SetPosition(diskPos.x, diskPos.y);
		qBert->SetPosition(diskPos.x, diskPos.y);
		HoppedOnTile({ 0, 0 });
	}

	m_Disks.erase(diskIt);
}

bool TileMapComponent::IsBlockIndexValid(const int2& blockIndex) const
{
	const auto x = blockIndex.first;
	const auto y = blockIndex.second;

	if (y < 0 || y >= (int)m_Tiles.size())
		return false;

	if (x < 0 || x >= (int)m_Tiles[y].size())
		return false;

	if (m_Tiles[y][x] == nullptr)
		return false;

	return true;
}

bool TileMapComponent::IsBlockIndexDisk(const int2& blockIndex) const
{
	return m_Disks.find(blockIndex) != m_Disks.end();
}

glm::vec2 TileMapComponent::IndexToTilePosition(const int2& blockIndex, bool absolutePosition) const
{
	// Offsets for fake perspective
	const int tileSize = TileComponent::GetTileSize();
	const int horizontalOffset = tileSize / 2;
	const int verticalOffset = tileSize * 3 / 4;

	auto result = glm::vec2(blockIndex.first * tileSize - blockIndex.second * horizontalOffset, blockIndex.second * verticalOffset);

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

void TileMapComponent::LoadLevelFromFile(const std::string& path, dae::SceneObject& parent)
{
	using namespace rapidjson;

	const auto fullPath = dae::ResourceManager::GetInstance().GetDataPath() + path;

	std::ifstream file{ fullPath };

	if (!file)
		return;

	IStreamWrapper istream{ file };

	Document doc;
	doc.ParseStream(istream);

	try {
		// -------------------- Read tiles ----------------------- //
		const auto requiredJumps = doc["RequiredJumps"].GetInt();
		const auto triangleSize = doc["TriangleSize"].GetInt();
		const auto resetOnJump = doc["ResetOnJump"].GetBool();

		m_Tiles.reserve(triangleSize);
		int nrOfTiles = 0;
		for (int i = 0; i < triangleSize; ++i)
		{
			m_Tiles.push_back(std::vector<std::unique_ptr<TileComponent>>());
			for (int j = 0; j <= i; ++j)
			{
				m_Tiles[i].push_back(
					std::make_unique<TileComponent>(IndexToTilePosition({ j, i }, false), requiredJumps, resetOnJump)
				);
			}
			nrOfTiles += (int)m_Tiles.size();
		}

		m_RequiredNrOfTilesToComplete = nrOfTiles;
		// ------------------------------------------------------- //

		// -------------------- Add disks ----------------------- //
		Value& diskArray = doc["Disks"];

		for (auto& value : diskArray.GetArray())
		{
			const auto xIdx = value["xIdx"].GetInt();
			const auto yIdx = value["yIdx"].GetInt();

			const auto idx = int2({ xIdx, yIdx });
			const auto diskPos = IndexToTilePosition(idx);
			const auto disk = std::make_shared<dae::SceneObject>(std::vector<dae::BaseComponent*>{}, glm::vec3{ diskPos.x, diskPos.y, 0 }, glm::vec2{ 2, 2 });
			disk->AddComponent(new DiskComponent());
			const auto renderComp = new dae::RenderComponent("Disk.png", { 16, 16 });
			disk->AddComponent(renderComp, true);

			parent.GetScene()->AddAfterInitialize(disk);
			m_Disks[idx] = disk;
		}
		// ------------------------------------------------------- //

	}
	catch (...)
	{
		dae::Logger::GetInstance().Print("Something went wrong reading in level");
		dae::Logger::GetInstance().SaveLog("Log.txt");
	}
}


std::size_t int2hash(const int2& x)
{
	return std::hash<int>()(x.first) ^ std::hash<int>()(x.second);
}

bool compareInt2(const int2& lhs, const int2& rhs)
{
	return lhs == rhs;
}
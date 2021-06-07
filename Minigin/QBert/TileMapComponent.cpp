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
#include "SubjectComponent.h"
#include "ScoreComponent.h"
#include <document.h>
#include "PosessedMovementComponent.h"
#include "SoundServiceLocator.h"
#include "SDL.h"

TileMapComponent::TileMapComponent()
	: m_Disks(14, int2hash, compareInt2)
	, m_LevelFolderName("Levels.json")
	, m_CurrentLevel(0)
	, m_SwitchLevel(false)
{
}

void TileMapComponent::Init(dae::SceneObject& parent)
{
	// Initialize this object
	m_pTransformRef = parent.GetFirstComponentOfType<dae::Transform>();
	m_pParentRef = &parent;

	m_pSubjectCompRef = parent.GetFirstComponentOfType<dae::SubjectComponent>();

	LoadLevelFromFile(m_LevelFolderName, m_CurrentLevel);
}

void TileMapComponent::Update(dae::SceneObject& parent)
{
	for (auto& tileArray : m_Tiles)
		for (auto& tile : tileArray)
			tile->Update(parent);

	if (m_SwitchLevel)
	{
		const auto nrOfDisksLeft = parent.GetScene()->GetNrOfObjectsByTag("disk");
		for (auto& qbert : parent.GetScene()->GetObjectsByTag("player"))
		{
			qbert->GetFirstComponentOfType<dae::ScoreComponent>()->IncreaseScore(50 * (int)nrOfDisksLeft);
		}

		dae::SoundServiceLocator::GetSoundService()->PlaySound("../Data/LevelComplete.wav", SDL_MIX_MAXVOLUME / 2);
		m_pSubjectCompRef->Broadcast(this, "level Changed");
		m_SwitchLevel = false;
		if (!LoadLevelFromFile(m_LevelFolderName, ++m_CurrentLevel))
		{
			dae::Logger::GetInstance().Print("Game over!");
			m_pSubjectCompRef->Broadcast(this, "game over");
		}
	}
}

bool TileMapComponent::HoppedOnTile(const int2& blockIndex, bool forceReverse)
{
	bool causedColourChange = false;
	auto tileChange = TileComponent::TileState::eProgress;

	if (!forceReverse)
		tileChange = m_Tiles[blockIndex.second][blockIndex.first]->HopOnTile();
	else
		tileChange = m_Tiles[blockIndex.second][blockIndex.first]->RevertTile();

	switch (tileChange)
	{
	case TileComponent::TileState::eNewlyCompleted:
		causedColourChange = true;
		++m_CompletedTiles;
		break;
	case TileComponent::TileState::eUncompleted:
		causedColourChange = true;
		--m_CompletedTiles;
		break;
	case TileComponent::TileState::eProgress:
		causedColourChange = true;
		break;
	default:
		break;
	}

	if (m_CompletedTiles >= m_RequiredNrOfTilesToComplete)
	{
		dae::Logger::GetInstance().Print("Level complete");
		m_SwitchLevel = true;
	}

	return causedColourChange;
}

void TileMapComponent::HoppedOnDisk(const int2& blockIndex, dae::SceneObject* qBert)
{
	const auto diskIt = m_Disks.find(blockIndex);
	const auto diskPos = IndexToTilePosition({ 0, 0 }) - glm::vec2{ 0, 30 };

	if (!diskIt->second.expired())
	{
		const auto diskPtr = diskIt->second.lock();
		diskPtr->GetFirstComponentOfType<DiskComponent>()->MoveToSpawn(qBert, diskPos);
		qBert->GetFirstComponentOfType<PosessedMovementComponent>()->SetEnabled(false);

		dae::SoundServiceLocator::GetSoundService()->PlaySound("../Data/Disk.wav", SDL_MIX_MAXVOLUME / 2);
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
	//dae::Logger::GetInstance().Print(std::to_string(causedColourChange.x) + " | " + std::to_string(causedColourChange.y));
	//dae::Logger::GetInstance().Print(R"(----------------------------------------------------------)");

	return result;
}

bool TileMapComponent::LoadLevelFromFile(const std::string& path, int level)
{
	using namespace rapidjson;

	const auto fullPath = dae::ResourceManager::GetInstance().GetDataPath() + path;

	std::ifstream file{ fullPath };

	if (!file)
		return false;

	DestroyLevel();

	IStreamWrapper istream{ file };

	Document doc;
	doc.ParseStream(istream);

	const auto NrOfLevels = doc["NrOfLevels"].GetInt();

	if (level >= NrOfLevels)
		return false;

	Value& jsonLevel = doc["Levels"].GetArray()[level];

	try {
		// -------------------- Read tiles ----------------------- //
		const auto requiredJumps = jsonLevel["RequiredJumps"].GetInt();
		const auto triangleSize = jsonLevel["TriangleSize"].GetInt();
		const auto resetOnJump = jsonLevel["ResetOnJump"].GetBool();

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

		// Initialize all tiles under the parent gameObject of the tileMap (we don't pass ownership though)
		for (auto& tileArray : m_Tiles)
			for (auto& tile : tileArray)
				tile->Init(*m_pParentRef);

		// ------------------------------------------------------- //

		// -------------------- Add disks ----------------------- //
		Value& diskArray = jsonLevel["Disks"];

		for (auto& value : diskArray.GetArray())
		{
			const auto xIdx = value["xIdx"].GetInt();
			const auto yIdx = value["yIdx"].GetInt();

			const auto idx = int2({ xIdx, yIdx });
			const auto diskPos = IndexToTilePosition(idx);
			const auto disk = std::make_shared<dae::SceneObject>(std::vector<dae::BaseComponent*>{}, glm::vec3{ diskPos.x, diskPos.y, 0 }, glm::vec2{ 2, 2 }, "disk");
			disk->AddComponent(new DiskComponent());
			const auto renderComp = new dae::RenderComponent("Disk.png", { 16, 16 });
			disk->AddComponent(renderComp, true);

			m_pParentRef->GetScene()->AddAfterInitialize(disk);
			m_Disks[idx] = disk;
		}
		// ------------------------------------------------------- //

	}
	catch (...)
	{
		dae::Logger::GetInstance().Print("Something went wrong reading in level");
		dae::Logger::GetInstance().SaveLog("Log.txt");
	}

	return true;
}

void TileMapComponent::DestroyLevel()
{
	for (auto& disk : m_Disks)
	{
		if(!disk.second.expired())
			m_pParentRef->GetScene()->Remove(disk.second.lock());
	}

	m_Tiles.clear();
	m_Disks.clear();
	m_CompletedTiles = 0;
	m_SwitchLevel = false;
}


std::size_t int2hash(const int2& x)
{
	return std::hash<int>()(x.first) ^ std::hash<int>()(x.second);
}

bool compareInt2(const int2& lhs, const int2& rhs)
{
	return lhs == rhs;
}
#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Logger.h"
#include "Scene.h"

void dae::SceneManager::FixedUpdate()
{
	if (m_ValidActiveScene && !m_ScenePaused)
	{
		m_ActiveScene->FixedUpdate();
	}
}

void dae::SceneManager::Update()
{
	if (m_ValidActiveScene && !m_ScenePaused)
	{
		m_ActiveScene->Update();
	}
}

void dae::SceneManager::Render()
{
	if (m_ValidActiveScene)
	{
		m_ActiveScene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	auto sceneIt = std::find_if(m_Scenes.cbegin(), m_Scenes.cend(), [&name](const std::shared_ptr<Scene>& scene)
		{
			return scene->GetName() == name;
		});

	if (sceneIt != m_Scenes.end())
	{
		Logger::GetInstance().Print("Scene " + name + " already exists! renaming...");
		return CreateScene(name + "_copy"); // Recursive in case name_copy also already exists
	}

	const auto scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);
	m_ActiveScene = scene;
	m_ValidActiveScene = true;
	return *scene;
}

bool dae::SceneManager::SetActiveScene(const std::string& name)
{
	auto sceneIt = std::find_if(m_Scenes.cbegin(), m_Scenes.cend(), [&name](const std::shared_ptr<Scene>& scene)
		{
			return scene->GetName() == name;
		});

	if (sceneIt != m_Scenes.end())
	{
		m_ActiveScene = *sceneIt;
		return true;
	}

	return false;
}

std::weak_ptr<dae::Scene> dae::SceneManager::GetActiveScene() const
{
	return m_ActiveScene;
}

void dae::SceneManager::SetScenePaused(bool paused)
{
	m_ScenePaused = paused;
}

bool dae::SceneManager::RemoveSceneByName(const std::string& name)
{	
	const auto it = std::remove_if(m_Scenes.begin(), m_Scenes.end(), [&name](const std::shared_ptr<Scene>& scene)
		{
			return scene->GetName() == name;
		});

	if (it == m_Scenes.end())
		return false;

	m_Scenes.erase(it, m_Scenes.end());
	return true;
}

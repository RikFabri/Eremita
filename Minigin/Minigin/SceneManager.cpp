#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::FixedUpdate()
{
	if (m_Initialized && !m_ScenePaused)
	{
		m_ActiveScene->FixedUpdate();
	}
}

void dae::SceneManager::Update()
{
	if (m_Initialized && !m_ScenePaused)
	{
		m_ActiveScene->Update();
	}
}

void dae::SceneManager::Render()
{
	if (m_Initialized)
	{
		m_ActiveScene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);
	m_ActiveScene = scene;
	m_Initialized = true;
	return *scene;
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	auto sceneIt = std::find_if(m_Scenes.begin(), m_Scenes.end(), [&name](const std::shared_ptr<Scene>& scene)
		{
			return scene->GetName() == name;
		});

	if (sceneIt != m_Scenes.end())
	{
		m_ActiveScene = *sceneIt;
	}
}

void dae::SceneManager::SetScenePaused(bool paused)
{
	m_ScenePaused = paused;
}
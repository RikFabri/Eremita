#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
#include <string>

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void FixedUpdate();
		void Update();
		void Render();

		// Sets the scene or returns false if it doesn't exist
		bool SetActiveScene(const std::string& name);
		// Removes a scene by name or returns false if it doesn't exist, the active scene is kept alive until a new active is set
		bool RemoveSceneByName(const std::string& name);
		void SetScenePaused(bool paused);
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		bool m_ValidActiveScene;
		bool m_ScenePaused;

		std::shared_ptr<Scene> m_ActiveScene;
		std::vector<std::shared_ptr<Scene>> m_Scenes;
	};
}

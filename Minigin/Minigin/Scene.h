#pragma once
#include "SceneManager.h"

namespace dae
{
	class SceneObject;
	class Scene
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(const std::shared_ptr<SceneObject>& object);
		// Adds object after initialization, it's your responsibility to initialize said object
		void AddAfterInitialize(const std::shared_ptr<SceneObject>& object);
		void Remove(const std::shared_ptr<SceneObject>& object);
		void Remove(const SceneObject* object);

		void Init();
		
		void FixedUpdate();
		void Update();
		void Render() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		std::vector<std::shared_ptr<SceneObject>> GetObjectsByTag(const std::string& tag);
	private: 
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector<std::shared_ptr<SceneObject>> m_Objects{};
		std::vector<std::shared_ptr<SceneObject>> m_ObjectsToBeAddedPostInit{};

		std::vector<std::shared_ptr<SceneObject>> m_ObjectsToBeRemovedSP{};
		std::vector<const SceneObject*> m_ObjectsToBeRemovedRP{};
	};

}

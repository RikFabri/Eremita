#include "MiniginPCH.h"
#include "Scene.h"
#include "SceneObject.h"
#include "Logger.h"
#include <algorithm>

using namespace dae;

std::vector<std::shared_ptr<SceneObject>> Scene::GetObjectsByTag(const std::string& tag)
{
	std::vector<std::shared_ptr<SceneObject>> objects;

	std::copy_if(m_Objects.begin(), m_Objects.end(), std::back_inserter(objects), 
		[&tag](std::shared_ptr<SceneObject>& sceneObject)
		{
			return sceneObject->GetTag() == tag;
		});

#if _DEBUG
	if (objects.empty())
	{
		Logger::GetInstance().Print("Couldn't find any " + tag);
		Logger::GetInstance().SaveLog("Log.txt");
	}
#endif // DEBUG


	return objects;
}

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(const std::shared_ptr<SceneObject>& object)
{
	object->SetScene(this);
	m_Objects.push_back(object);
}

void dae::Scene::AddAfterInitialize(const std::shared_ptr<SceneObject>& object)
{
	object->SetScene(this);
	object->Init();
	m_ObjectsToBeAddedPostInit.emplace_back(object);
}

void dae::Scene::Remove(const std::shared_ptr<SceneObject>& object)
{
	m_ObjectsToBeRemovedSP.push_back(object);
}

void dae::Scene::Remove(const SceneObject* object)
{
	m_ObjectsToBeRemovedRP.push_back(object);
}

void Scene::Init()
{
	for (auto& object : m_Objects)
		object->Init();

	std::move(m_ObjectsToBeAddedPostInit.begin(), m_ObjectsToBeAddedPostInit.end(), std::back_inserter(m_Objects));
	m_ObjectsToBeAddedPostInit.clear();
}

void Scene::FixedUpdate()
{
	for(auto& object : m_Objects)
		object->FixedUpdate();
}

void Scene::Update()
{
	for (auto& object : m_Objects)
		object->Update();

	// Actually remove objects that got removed during update
	if (!m_ObjectsToBeRemovedSP.empty())
	{
		auto endIt = m_Objects.end();
		for (auto object : m_ObjectsToBeRemovedSP)
		{
			endIt = std::remove(m_Objects.begin(), endIt, object);
		}
		m_Objects.erase(endIt, m_Objects.end());
		m_ObjectsToBeRemovedSP.clear();
	}

	if (!m_ObjectsToBeRemovedRP.empty())
	{
		auto endIt = m_Objects.end();
		for (auto object : m_ObjectsToBeRemovedRP)
		{
			endIt = std::remove_if(m_Objects.begin(), endIt, [object](const std::shared_ptr<SceneObject>& ptr)
				{
					return ptr.get() == object;
				});
		}
		m_Objects.erase(endIt, m_Objects.end());
		m_ObjectsToBeRemovedRP.clear();
	}

	// Add objects created past initialization
	if (!m_ObjectsToBeAddedPostInit.empty())
	{
		std::move(m_ObjectsToBeAddedPostInit.begin(), m_ObjectsToBeAddedPostInit.end(), std::back_inserter(m_Objects));
		m_ObjectsToBeAddedPostInit.clear();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
		object->Render();
}


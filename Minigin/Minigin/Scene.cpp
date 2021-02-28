#include "MiniginPCH.h"
#include "Scene.h"
#include "SceneObject.h"
#include <algorithm>

using namespace dae;

//unsigned int Scene::m_IdCounter = 0;

std::vector<std::shared_ptr<SceneObject>> Scene::GetObjectsByTag(const std::string& tag)
{
	std::vector<std::shared_ptr<SceneObject>> objects;

	std::copy_if(m_Objects.begin(), m_Objects.end(), std::back_inserter(objects), 
		[&tag](std::shared_ptr<SceneObject>& sceneObject)
		{
			return sceneObject->GetTag() == tag;
		});

	return objects;
}

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(const std::shared_ptr<SceneObject>& object)
{
	object->SetScene(this);
	m_Objects.push_back(object);
}

void Scene::Init()
{
	for (auto& object : m_Objects)
		object->Init();
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
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
		object->Render();
}


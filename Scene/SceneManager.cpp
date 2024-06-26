#include "SceneManager.h"

SceneManager* SceneManager::m_instancePtr = nullptr;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

shared_ptr<Scene> SceneManager::MakeScene()
{
	m_currentScene = make_shared<Scene>();
	m_sceneList.push_back(m_currentScene);

	return m_currentScene;
}

shared_ptr<Scene> SceneManager::GetCurrentScene()
{
	return m_currentScene;
}
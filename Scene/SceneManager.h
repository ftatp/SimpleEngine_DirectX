#pragma once
#include "Scene.h"

#include <memory>
#include <vector>

//using namespace scene;

class SceneManager
{
public:
	static SceneManager* GetInstance()
	{
		if (m_instancePtr == nullptr)
		{
			m_instancePtr = new SceneManager();
		}

		return m_instancePtr;
	}

	SceneManager();
	~SceneManager();

	shared_ptr<scene::Scene> MakeScene();
	shared_ptr<scene::Scene> GetCurrentScene();

private:
	static SceneManager* m_instancePtr;

	shared_ptr<scene::Scene> m_currentScene;
	vector<shared_ptr<scene::Scene>> m_sceneList;
};


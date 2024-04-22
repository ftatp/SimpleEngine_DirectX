#pragma once
#include "Scene.h"

#include <memory>
#include <vector>

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

	shared_ptr<Scene> MakeScene();
	shared_ptr<Scene> GetCurrentScene();

private:
	static SceneManager* m_instancePtr;

	shared_ptr<Scene> m_currentScene;
	vector<shared_ptr<Scene>> m_sceneList;
};


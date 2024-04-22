#pragma once
#include "Object/Object.h"
#include "Object/GeometryConstructor.h"

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

	void MakeObject();
	vector<shared_ptr<object::Object>> GetObjectList();

	void SetObjectTranslation(int objectIndex, Vector3 translation);
	void SetObjectRotation(int objectIndex, Vector3 rotation);
	void SetObjectScale(int objectIndex, Vector3 scale);

private:
	static SceneManager* m_instancePtr;

	vector<shared_ptr<object::Object>> m_objectList;
};


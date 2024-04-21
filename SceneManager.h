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

	void SetObjectTranslation(int objectIndex, Matrix translation);
	void SetObjectRotation(int objectIndex, Matrix rotation);
	void SetObjectScale(int objectIndex, Matrix scale);

private:
	static SceneManager* m_instancePtr;

	vector<shared_ptr<object::Object>> m_objectList;
};


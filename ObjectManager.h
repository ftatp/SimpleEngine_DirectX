#pragma once
#include "Object/Object.h"
#include "Object/GeometryConstructor.h"

#include <memory>
#include <vector>

class ObjectManager
{
public:
	static ObjectManager* GetInstance()
	{
		if (m_instancePtr == nullptr)
		{
			m_instancePtr = new ObjectManager();
		}

		return m_instancePtr;
	}

	ObjectManager();
	~ObjectManager();

	void MakeObject();
	vector<shared_ptr<object::Object>> GetObjectList();

	void SetObjectTranslation(int objectIndex, Matrix translation);
	void SetObjectRotation(int objectIndex, Matrix rotation);
	void SetObjectScale(int objectIndex, Matrix scale);

private:
	static ObjectManager* m_instancePtr;

	vector<shared_ptr<object::Object>> m_objectList;
};


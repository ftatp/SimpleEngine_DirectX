#include "Scene.h"

namespace scene
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::MakeObject()
	{
		shared_ptr<object::Object> newObject = make_shared<object::Object>();

		m_objectList.push_back(newObject);
	}

	vector<shared_ptr<object::Object>> Scene::GetObjectList()
	{
		return m_objectList;
	}

	void Scene::SetObjectTranslation(int objectIndex, Vector3 translation)
	{
		auto object = m_objectList[objectIndex];
		object->SetTranslation(translation);
	}

	void Scene::SetObjectRotation(int objectIndex, Vector3 rotation)
	{
		auto object = m_objectList[objectIndex];
		object->SetRotation(rotation);
	}

	void Scene::SetObjectScale(int objectIndex, Vector3 scale)
	{
		auto object = m_objectList[objectIndex];
		object->SetScale(scale);
	}
}
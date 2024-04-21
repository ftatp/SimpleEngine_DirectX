#include "SceneManager.h"

SceneManager* SceneManager::m_instancePtr = nullptr;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::MakeObject()
{
	shared_ptr<object::Object> newObject = make_shared<object::Object>();

	m_objectList.push_back(newObject);
}

vector<shared_ptr<object::Object>> SceneManager::GetObjectList()
{
	return m_objectList;
}

void SceneManager::SetObjectTranslation(int objectIndex, Matrix translation)
{
	auto object = m_objectList[objectIndex];
	object->SetTranslation(translation);
}

void SceneManager::SetObjectRotation(int objectIndex, Matrix rotation)
{
	auto object = m_objectList[objectIndex];
	object->SetRotation(rotation);
}

void SceneManager::SetObjectScale(int objectIndex, Matrix scale)
{
	auto object = m_objectList[objectIndex];
	object->SetScale(scale);
}
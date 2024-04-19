#include "ObjectManager.h"

ObjectManager* ObjectManager::m_instancePtr = nullptr;

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::MakeObject()
{
	shared_ptr<object::Object> newObject = make_shared<object::Object>();

	m_objectList.push_back(newObject);
}

vector<shared_ptr<object::Object>> ObjectManager::GetObjectList()
{
	return m_objectList;
}

void ObjectManager::SetObjectTranslation(int objectIndex, Matrix translation)
{
	auto object = m_objectList[objectIndex];
	object->SetTranslation(translation);
}

void ObjectManager::SetObjectRotation(int objectIndex, Matrix rotation)
{
	auto object = m_objectList[objectIndex];
	object->SetRotation(rotation);
}

void ObjectManager::SetObjectScale(int objectIndex, Matrix scale)
{
	auto object = m_objectList[objectIndex];
	object->SetScale(scale);
}
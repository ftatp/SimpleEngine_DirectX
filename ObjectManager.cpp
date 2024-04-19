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
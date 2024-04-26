#pragma once
#include "Object/Object.h"
#include "Object/GeometryConstructor.h"

#include <memory>
#include <vector>

namespace scene 
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		void MakeObject();
		vector<shared_ptr<object::Object>> GetObjectList();

		void SetObjectTranslation(int objectIndex, Vector3 translation);
		void SetObjectRotation(int objectIndex, Vector3 rotation);
		void SetObjectScale(int objectIndex, Vector3 scale);

	private:
		vector<shared_ptr<object::Object>> m_objectList;

	};
}
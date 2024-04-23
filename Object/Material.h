#pragma once
#include <directxtk/SimpleMath.h>

using namespace DirectX::SimpleMath;

namespace object 
{
	class Material
	{
	public:
		Material();
		~Material();

		Vector3 GetAmbient();
		void SetAmbient(Vector3 ambient);
		Vector3 GetDiffuse();
		void SetDiffuse(Vector3 diffuse);
		Vector3 GetSpecular();
		void SetSpecular(Vector3 specular);
		float GetShininess();
		void SetShininess(float shininess);

	private:
		Vector3 m_ambient;
		float padding1 = 0.0f;
		Vector3 m_diffuse;
		float padding2 = 0.0f;
		Vector3 m_specular;
		float m_shininess = 10.0f;
	};
}
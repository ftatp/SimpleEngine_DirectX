#include "Material.h"

#include <iostream>

namespace object
{
	Material::Material()
	{
	}

	Material::~Material()
	{
	}


	Vector3 Material::GetAmbient()
	{
		return m_ambient;
	}
	
	void Material::SetAmbient(Vector3 ambient)
	{
		m_ambient = ambient;
	}
	
	Vector3 Material::GetDiffuse()
	{
		return m_diffuse;
	}
	
	void Material::SetDiffuse(Vector3 diffuse)
	{
		m_diffuse = diffuse;
	}
	
	Vector3 Material::GetSpecular()
	{
		return m_specular;
	}
	
	void Material::SetSpecular(Vector3 specular)
	{
		m_specular = specular;
	}
	
	float Material::GetShininess()
	{
		return m_shininess;
	}
	
	void Material::SetShininess(float shininess)
	{
		m_shininess = shininess;
	}
}
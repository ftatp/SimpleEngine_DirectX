#include "Mesh.h"

namespace object
{
	Mesh::Mesh()
	{
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::SetMeshData(vector<Vector3> vertices, vector<uint16_t> indices)
	{
		this->vertices = vertices;
		this->indices = indices;
	}

	vector<Vector3> Mesh::GetVertices()
	{
		return vertices;
	}

	vector<uint16_t> Mesh::GetIndices()
	{
		return indices;
	}
}
#include "Mesh.h"

namespace object
{
	Mesh::Mesh()
	{
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::SetMeshData(vector<Vertex> vertices, vector<uint16_t> indices)
	{
		this->vertices = vertices;
		this->indices = indices;
	}

	vector<object::Vertex> Mesh::GetVertices()
	{
		return vertices;
	}

	vector<uint16_t> Mesh::GetIndices()
	{
		return indices;
	}
}
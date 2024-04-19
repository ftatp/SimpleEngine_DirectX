#include "GeometryConstructor.h"

namespace object 
{
	shared_ptr<Mesh> CreateTriangle()
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();

		vector<Vector3> vertices;
		vector<uint16_t> indices;

		vertices.push_back({ -1.0, -1.0, 0.0 });
		vertices.push_back({ 0.0, 1.0, 0.0 });
		vertices.push_back({ 1.0, -1.0, 0.0 });

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		mesh->SetMeshData(vertices, indices);

		return mesh;
	}
}
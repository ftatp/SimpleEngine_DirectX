#include "GeometryConstructor.h"

namespace object 
{
	shared_ptr<Mesh> CreateTriangle()
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
        const int numOfVertices = 3;

		vector<Vector3> positions;
        vector<Vector3> normals;
        vector<Vector3> colors;
        vector<Vector2> textureCoords;
		vector<uint16_t> indices;

        positions.push_back({ -1.0, -1.0, 0.0 });
        positions.push_back({ 0.0, 1.0, 0.0 });
        positions.push_back({ 1.0, -1.0, 0.0 });

        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));

        colors.push_back(Vector3(0.0f, 0.0f, 0.5f));
        colors.push_back(Vector3(0.0f, 0.0f, 0.5f));
        colors.push_back(Vector3(0.0f, 0.0f, 0.5f));

        textureCoords.push_back(Vector2(1.0f, 0.0f));
        textureCoords.push_back(Vector2(0.0f, 0.5f));
        textureCoords.push_back(Vector2(1.0f, 1.0f));

        vector<object::Vertex> vertices;
        for (size_t i = 0; i < numOfVertices; i++)
        {
            Vertex vertex = { 
                positions[i], 
                normals[i], 
                colors[i],
                textureCoords[i]
            };

            vertices.push_back(vertex);
        }

        indices = { 0, 1, 2 };
		mesh->SetMeshData(vertices, indices);

		return mesh;
	}

	shared_ptr<Mesh> CreateSquare()
	{
        shared_ptr<Mesh> mesh = make_shared<Mesh>();
        const int numOfVertices = 4;

        vector<Vector3> positions;
        vector<uint16_t> indices;
        vector<Vector3> colors;
        vector<Vector3> normals;
        vector<Vector2> textureCoords; // ÅØ½ºÃç ÁÂÇ¥

        const float scale = 1.0f;

        positions.push_back(Vector3(-1.0f, 1.0f, 0.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, 0.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, 0.0f) * scale);
        positions.push_back(Vector3(-1.0f, -1.0f, 0.0f) * scale);

        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));

        colors.push_back(Vector3(0.0f, 0.0f, 0.7f));
        colors.push_back(Vector3(0.0f, 0.0f, 0.7f));
        colors.push_back(Vector3(0.0f, 0.0f, 0.7f));
        colors.push_back(Vector3(0.0f, 0.0f, 0.7f));

        textureCoords.push_back(Vector2(0.0f, 0.0f));
        textureCoords.push_back(Vector2(1.0f, 0.0f));
        textureCoords.push_back(Vector2(1.0f, 1.0f));
        textureCoords.push_back(Vector2(0.0f, 1.0f));

        vector<object::Vertex> vertices;
        for (size_t i = 0; i < numOfVertices; i++) 
        {
            Vertex vertex = {
                positions[i],
                normals[i],
                colors[i],
                textureCoords[i]
            };

            vertices.push_back(vertex);
        }

        indices = {
            0, 1, 2, 
            0, 2, 3,
        };

        mesh->SetMeshData(vertices, indices);

        return mesh;
	}
}
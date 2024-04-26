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
        vector<Vector3> colors;
        vector<Vector3> normals;
        vector<Vector2> textureCoords;
        vector<uint16_t> indices;

        positions.push_back(Vector3(-1.0f, 1.0f, 0.0f));
        positions.push_back(Vector3(1.0f, 1.0f, 0.0f));
        positions.push_back(Vector3(1.0f, -1.0f, 0.0f));
        positions.push_back(Vector3(-1.0f, -1.0f, 0.0f));

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

    shared_ptr<Mesh> CreateBox()
    {
        shared_ptr<Mesh> mesh = make_shared<Mesh>();
        const int numOfVertices = 24;

        vector<Vector3> positions;
        vector<Vector3> colors;
        vector<Vector3> normals;
        vector<Vector2> textureCoords;
        vector<uint16_t> indices;

        const float scale = 1.0f;

        // Up
        positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);

        normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
        normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
        normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
        normals.push_back(Vector3(0.0f, 1.0f, 0.0f));

        colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
        colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
        colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
        colors.push_back(Vector3(1.0f, 0.0f, 0.0f));

        textureCoords.push_back(Vector2(0.0f, 0.0f));
        textureCoords.push_back(Vector2(1.0f, 0.0f));
        textureCoords.push_back(Vector2(1.0f, 1.0f));
        textureCoords.push_back(Vector2(0.0f, 1.0f));

        // Down
        positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
        
        normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
        normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
        normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
        normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
        
        colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
        
        textureCoords.push_back(Vector2(0.0f, 0.0f));
        textureCoords.push_back(Vector2(1.0f, 0.0f));
        textureCoords.push_back(Vector2(1.0f, 1.0f));
        textureCoords.push_back(Vector2(0.0f, 1.0f));

        // Front
        positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
        positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);

        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));

        colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 0.0f, 1.0f));

        textureCoords.push_back(Vector2(0.0f, 0.0f));
        textureCoords.push_back(Vector2(1.0f, 0.0f));
        textureCoords.push_back(Vector2(1.0f, 1.0f));
        textureCoords.push_back(Vector2(0.0f, 1.0f));

        // Back
        positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
        
        normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
        
        colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 1.0f));

        textureCoords.push_back(Vector2(0.0f, 0.0f));
        textureCoords.push_back(Vector2(1.0f, 0.0f));
        textureCoords.push_back(Vector2(1.0f, 1.0f));
        textureCoords.push_back(Vector2(0.0f, 1.0f));

        // Left
        positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
        
        normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
        normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
        normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
        normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
        
        colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
        colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
        colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
        colors.push_back(Vector3(1.0f, 1.0f, 0.0f));

        textureCoords.push_back(Vector2(0.0f, 0.0f));
        textureCoords.push_back(Vector2(1.0f, 0.0f));
        textureCoords.push_back(Vector2(1.0f, 1.0f));
        textureCoords.push_back(Vector2(0.0f, 1.0f));

        // Right
        positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
        
        normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
        normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
        normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
        normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
        
        colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(1.0f, 0.0f, 1.0f));

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
            0,  1,  2,  0,  2,  3,  // 윗면
            4,  5,  6,  4,  6,  7,  // 아랫면
            8,  9,  10, 8,  10, 11, // 앞면
            12, 13, 14, 12, 14, 15, // 뒷면
            16, 17, 18, 16, 18, 19, // 왼쪽
            20, 21, 22, 20, 22, 23  // 오른쪽
        };

        mesh->SetMeshData(vertices, indices);

        return mesh;
    }

    shared_ptr<Mesh> CreateGrid(const float width, const float height, const int numSlices, const int numStacks) 
    {
        shared_ptr<Mesh> mesh = make_shared<Mesh>();
        vector<Vertex> vertices;
        vector<uint16_t> indices;

        float dx = width / numSlices;
        float dy = height / numStacks;

        for (int i = 0; i <= numStacks; i++)
        {
            Vector3 startPoint = Vector3(-width * 0.5, -height * 0.5 + dy * i, 0);

            for (int j = 0; j <= numSlices; j++)
            {
                Vertex currentVertex;
                currentVertex.position = startPoint + Vector3(dx * j, 0.0f, 0.0f);
                currentVertex.normal = Vector3(0.0f, 0.0f, -1.0f);
                currentVertex.color = Vector3(0.0f, 0.0f, 0.5f);
                currentVertex.textureCoord = Vector2(dx * j, 1 - dy * i);

                vertices.push_back(currentVertex);
            }
        }

        for (uint16_t i = 0; i < numStacks; i++)
        {
            for (uint16_t j = 0; j < numSlices; j++)
            {
                uint16_t startIndex = j + i * (numSlices + 1);
                indices.push_back(startIndex);
                indices.push_back(startIndex + (numSlices + 1));
                indices.push_back(startIndex + (numSlices + 1) + 1);

                indices.push_back(startIndex);
                indices.push_back(startIndex + (numSlices + 1) + 1);
                indices.push_back(startIndex + 1);
            }
        }

        mesh->SetMeshData(vertices, indices);

        return mesh;
    }

    shared_ptr<Mesh> CreateCylinder(const float bottomRadius, const float topRadius, float height, int sliceCount)
    {
        const float dTheta = -DirectX::XM_2PI / float(sliceCount);

        shared_ptr<Mesh> mesh = make_shared<Mesh>();

        vector<Vertex> vertices;

        for (int i = 0; i <= sliceCount; i++)
        {
            Vertex vertex;
            vertex.position = Vector3(cos(dTheta * i), 0, sin(dTheta * i)) * bottomRadius;
            vertex.normal = Vector3(cos(dTheta * i), 0, sin(dTheta * i));
            vertex.color = Vector3(0.0f, 0.0f, 0.5f);
            vertex.textureCoord = Vector2(i / (float)sliceCount, 1.0f);
            vertices.push_back(vertex);
        }

        for (int i = 0; i <= sliceCount; i++)
        {
            Vertex vertex;
            vertex.position = Vector3(cos(dTheta * i), height, sin(dTheta * i)) * topRadius;
            vertex.normal = Vector3(cos(dTheta * i), 0, sin(dTheta * i));
            vertex.color = Vector3(0.0f, 0.0f, 0.5f);
            vertex.textureCoord = Vector2(i / (float)sliceCount, 0.0f);
            vertices.push_back(vertex);
        }

        vector<uint16_t> indices;

        for (int i = 0; i < sliceCount; i++)
        {
            // TODO: 삼각형 두 개 씩
            indices.push_back(i);
            indices.push_back(i + sliceCount + 1);
            indices.push_back(i + sliceCount + 2);

            indices.push_back(i);
            indices.push_back(i + sliceCount + 2);
            indices.push_back(i + 1);
        }

        mesh->SetMeshData(vertices, indices);

        return mesh;
    }

    shared_ptr<Mesh> CreateSphere(const float radius, const int numSlices, const int numStacks) 
    {
        const float dTheta = -DirectX::XM_2PI / float(numSlices);
        const float dPhi = -DirectX::XM_PI / float(numStacks);

        shared_ptr<Mesh> mesh = make_shared<Mesh>();

        vector<Vertex> vertices;
        for (int j = 0; j <= numStacks; j++) 
        {
            float y = -radius + 2.0f * radius / numStacks * j;
            float x = sqrt(radius * radius - y * y);
            Vector3 stackStartPoint = Vector3(x, y, 0.0f);
            for (int i = 0; i <= numSlices; i++) {
                Vertex vertex;

                // 시작점을 x-z 평면에서 회전시키면서 원을 만드는 구조
                vertex.position = Vector3::Transform(stackStartPoint, Matrix::CreateRotationY(dTheta * float(i)));

                vertex.normal = vertex.position; // 원점이 구의 중심
                vertex.normal.Normalize();
                vertex.textureCoord = Vector2(float(i) / numSlices, 1.0f - float(j) / numStacks);

                vertices.push_back(vertex);
            }
        }

        vector<uint16_t> indices;

        for (int j = 0; j < numStacks; j++) {

            const int offset = (numSlices + 1) * j;

            for (int i = 0; i < numSlices; i++) {

                indices.push_back(offset + i);
                indices.push_back(offset + i + numSlices + 1);
                indices.push_back(offset + i + 1 + numSlices + 1);

                indices.push_back(offset + i);
                indices.push_back(offset + i + 1 + numSlices + 1);
                indices.push_back(offset + i + 1);
            }
        }


        mesh->SetMeshData(vertices, indices);

        return mesh;
    }
}
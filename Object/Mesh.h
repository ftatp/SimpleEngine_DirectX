#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgidebug.h>
#include <directxtk/SimpleMath.h>

#include <vector>

using namespace DirectX::SimpleMath;
using namespace std;

namespace object 
{
	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
		Vector3 color;
		Vector2 textureCoord;
	};

	class Mesh
	{
	public:
		Mesh();
		~Mesh();

		void SetMeshData(vector<object::Vertex> vertices, vector<uint16_t> indices);

		vector<object::Vertex> GetVertices();
		vector<uint16_t> GetIndices();

	private:
		vector<object::Vertex> vertices;
		vector<uint16_t> indices;
	};
}
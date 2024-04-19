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
	class Mesh
	{
	public:
		Mesh();
		~Mesh();

		void SetMeshData(vector<Vector3> vertices, vector<uint16_t> indices);

		vector<Vector3> GetVertices();
		vector<uint16_t> GetIndices();

	private:
		vector<Vector3> vertices;
		vector<uint16_t> indices;
	};
}
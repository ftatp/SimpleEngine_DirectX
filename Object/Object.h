#pragma once

#include "Mesh.h"
#include "GeometryConstructor.h"
#include <wrl.h>

using namespace Microsoft::WRL;

struct ConstantData
{
	Matrix model;
	Matrix view;
	Matrix projection;
};

namespace object 
{
	class Object
	{
	public:
		Object();
		~Object();

		void SetMesh(shared_ptr<Mesh> mesh);
		void SetVertexShader(LPCWSTR vertexShaderFile);
		void SetPixelShader(LPCWSTR pixelShaderFile);
		Matrix GetModelTransform();
		ConstantData GetConstantData();
		void SetConstantData(ConstantData constantData);

		ComPtr<ID3D11Buffer> GetVertexBuffer();
		ComPtr<ID3D11Buffer> GetIndexBuffer();
		ComPtr<ID3D11Buffer> GetConstantBuffer();

		ComPtr<ID3D11VertexShader> GetVertexShader();
		ComPtr<ID3D11InputLayout> GetInputLayout();
		ComPtr<ID3D11PixelShader> GetPixelShader();

	private:
		void CreateVertexBuffer();
		void CreateIndiceBuffer();
		void CreateConstantBuffer();

		void CreateVertexShader();
		void CreatePixelShader();

		shared_ptr<Mesh> m_mesh;
		
		Matrix m_translation = Matrix();
		Matrix m_rotation = Matrix();
		Matrix m_scale = Matrix();
		ConstantData m_constantData = {};

		ComPtr<ID3D11Buffer> m_vertexBuffer;
		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11Buffer> m_constantBuffer;

		LPCWSTR m_vertexShaderFile;
		LPCWSTR m_pixelShaderFile;

		ComPtr<ID3D11VertexShader> m_vertexShader;
		ComPtr<ID3D11InputLayout> m_inputLayout;
		ComPtr<ID3D11PixelShader> m_pixelShader;
	};
}
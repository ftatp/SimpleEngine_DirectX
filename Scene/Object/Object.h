#pragma once

#include "Mesh.h"
#include "Material.h"
#include "GeometryConstructor.h"
#include <wrl.h>

using namespace Microsoft::WRL;

struct VertexShaderConstantData
{
	Matrix model;
	Matrix view;
	Matrix projection;
};

struct PixelShaderConstantData {
	bool useTexture; // 1 but padded to 4
	Vector3 eyePosition;
	//int lightMode; // 4
	//float padding[3];
	object::Material material;
//	Light lights[MAX_LIGHTS];
};

namespace object 
{
	class Object
	{
	public:
		Object();
		~Object();

		shared_ptr<Mesh> GetMesh();
		void SetMesh(shared_ptr<Mesh> mesh);
		shared_ptr<Material> GetMaterial();
		void SetMaterial(shared_ptr<Material> material);

		
		void SetVertexShader(LPCWSTR vertexShaderFile);
		void SetPixelShader(LPCWSTR pixelShaderFile);
		VertexShaderConstantData GetVertexShaderConstantData();
		void SetVertexShaderConstantData(VertexShaderConstantData vertexShaderConstantData);
		PixelShaderConstantData GetPixelShaderConstantData();
		void SetPixelShaderConstantData(PixelShaderConstantData pixelShaderConstantData);


		Vector3* GetTranslationPtr();
		Vector3* GetRotationPtr();
		Vector3* GetScalePtr();

		Matrix GetModelTransform();
		void SetTranslation(Vector3 translation);
		void SetRotation(Vector3 rotation);
		void SetScale(Vector3 scale);
		

		ComPtr<ID3D11Buffer> GetVertexBuffer();
		ComPtr<ID3D11Buffer> GetIndexBuffer();
		ComPtr<ID3D11Buffer> GetVertexShaderConstantBuffer();
		ComPtr<ID3D11Buffer> GetPixelShaderConstantBuffer();
		
		ComPtr<ID3D11VertexShader> GetVertexShader();
		ComPtr<ID3D11InputLayout> GetInputLayout();
		ComPtr<ID3D11PixelShader> GetPixelShader();

	private:
		void CreateVertexBuffer();
		void CreateIndiceBuffer();
		template <typename T>
		void CreateConstantBuffer(T& constantData, ComPtr<ID3D11Buffer>& constantBuffer);
		template <typename T>
		void UpdateConstantBuffer(T& constantData, ComPtr<ID3D11Buffer>& constantBuffer);
		
		void CreateVertexShader();
		void CreatePixelShader();

		shared_ptr<Mesh> m_mesh;
		shared_ptr<Material> m_material;
		
		Vector3 m_translation = Vector3(0.0f);
		Vector3 m_rotation = Vector3(0.0f);
		Vector3 m_scale = Vector3(1.0f);

		VertexShaderConstantData m_vertexShaderConstantData = {};
		PixelShaderConstantData m_pixelShaderConstantData = {};

		ComPtr<ID3D11Buffer> m_vertexBuffer;
		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11Buffer> m_vertexShaderConstantBuffer;
		ComPtr<ID3D11Buffer> m_pixelShaderConstantBuffer;

		LPCWSTR m_vertexShaderFile;
		LPCWSTR m_pixelShaderFile;

		ComPtr<ID3D11VertexShader> m_vertexShader;
		ComPtr<ID3D11InputLayout> m_inputLayout;
		ComPtr<ID3D11PixelShader> m_pixelShader;
	};
}
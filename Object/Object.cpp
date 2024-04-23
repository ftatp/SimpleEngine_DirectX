#include "Object.h"
#include "../WindowManager.h"

#include <iostream>

namespace object
{
	Object::Object()
	{
        //m_mesh = CreateTriangle();
        m_mesh = CreateGrid(2.0f, 1.7f, 5, 3);
        //m_mesh = CreateBox();
        //m_mesh = CreateCylinder(1.0f, 0.7f, 2.0f, 20);
        m_vertexShaderFile = L"VertexShader.hlsl";
        m_pixelShaderFile = L"PixelShader.hlsl";

        //m_translation = Matrix::CreateTranslation(Vector3(0.0f));
        //m_rotation = Matrix::CreateRotationX(0.0f) * Matrix::CreateRotationY(0.0f) * Matrix::CreateRotationZ(0.0f);
        //m_scale = Matrix::CreateScale(Vector3(1.0f));

        m_translation = Vector3(0.0f);
        m_rotation = Vector3(0.0f);
        m_scale = Vector3(1.0f, 1.0f, 1.0f);

        m_constantData.model = Matrix();
        m_constantData.view = Matrix();
        m_constantData.projection = Matrix();

        CreateVertexBuffer();
        CreateIndiceBuffer();
        CreateConstantBuffer();
        CreateVertexShader();
        CreatePixelShader();
	}

	Object::~Object()
	{
        
	}

    shared_ptr<Mesh> Object::GetMesh()
    {
        return m_mesh;
    }

	void Object::SetMesh(shared_ptr<Mesh> mesh)
	{
		m_mesh = mesh;
        CreateVertexBuffer();
        CreateIndiceBuffer();
	}

    void Object::SetVertexShader(LPCWSTR vertexShaderFile)
    {
        m_vertexShaderFile = vertexShaderFile;
        CreateVertexShader();
    }

    void Object::SetPixelShader(LPCWSTR pixelShaderFile)
    {
        m_pixelShaderFile = pixelShaderFile;
        CreatePixelShader();
    }

    Vector3* Object::GetTranslationPtr()
    {
        return &m_translation;
    }
    
    Vector3* Object::GetRotationPtr()
    {
        return &m_rotation;
    }
    
    Vector3* Object::GetScalePtr()
    {
        return &m_scale;
    }

    Matrix Object::GetModelTransform()
    {
        return Matrix::CreateScale(m_scale) 
            * Matrix::CreateRotationZ(m_rotation.z)
            * Matrix::CreateRotationY(m_rotation.y)
            * Matrix::CreateRotationX(m_rotation.x)
            * Matrix::CreateTranslation(m_translation);
    }

    void Object::SetTranslation(Vector3 translation)
    {
        m_translation = translation;
    }

    void Object::SetRotation(Vector3 rotation)
    {
        m_rotation = rotation;
    }
    
    void Object::SetScale(Vector3 scale)
    {
        m_scale = scale;
    } 

    ConstantData Object::GetConstantData()
    {
        return m_constantData;
    }

    void Object::SetConstantData(ConstantData constantData)
    {
        m_constantData = constantData;
    }

    ComPtr<ID3D11Buffer> Object::GetVertexBuffer()
    {
        return m_vertexBuffer;
    }

    ComPtr<ID3D11Buffer> Object::GetIndexBuffer()
    {
        return m_indexBuffer;
    }
    
    ComPtr<ID3D11Buffer> Object::GetConstantBuffer()
    {
        return m_constantBuffer;
    }
    
    ComPtr<ID3D11VertexShader> Object::GetVertexShader()
    {
        return m_vertexShader;
    }
    
    ComPtr<ID3D11InputLayout> Object::GetInputLayout()
    {
        return m_inputLayout;
    }
    
    ComPtr<ID3D11PixelShader> Object::GetPixelShader() 
    {
        return m_pixelShader;
    }

    void Object::CreateVertexBuffer()
    {
        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
        vertexBufferDesc.ByteWidth = UINT(sizeof(object::Vertex) * m_mesh->GetVertices().size());
        vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.StructureByteStride = UINT(sizeof(object::Vertex));

        D3D11_SUBRESOURCE_DATA vertexBufferSubresource;
        ZeroMemory(&vertexBufferSubresource, sizeof(D3D11_SUBRESOURCE_DATA));
        auto vertices = m_mesh->GetVertices();
        vertexBufferSubresource.pSysMem = vertices.data();

        const HRESULT isVertexBufferCreated =
            WindowManager::GetInstance()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferSubresource, m_vertexBuffer.GetAddressOf());

        if (FAILED(isVertexBufferCreated)) {
            std::cout << __FUNCTION__ << "failed. " << std::hex << isVertexBufferCreated << std::endl;
        };
    }

    void Object::CreateIndiceBuffer()
    {
        D3D11_BUFFER_DESC indexBufferDesc;
        ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
        indexBufferDesc.ByteWidth = UINT(sizeof(uint16_t) * m_mesh->GetIndices().size());
        indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.StructureByteStride = UINT(sizeof(uint16_t));

        D3D11_SUBRESOURCE_DATA indexBufferSubresource;
        ZeroMemory(&indexBufferSubresource, sizeof(D3D11_SUBRESOURCE_DATA));
        auto indices = m_mesh->GetIndices();
        indexBufferSubresource.pSysMem = indices.data();

        const HRESULT isIndexBufferCreated =
            WindowManager::GetInstance()->GetDevice()->CreateBuffer(&indexBufferDesc, &indexBufferSubresource, m_indexBuffer.GetAddressOf());

        if (FAILED(isIndexBufferCreated)) {
            std::cout << __FUNCTION__ << "failed. " << std::hex << isIndexBufferCreated << std::endl;
        };
    }

    void Object::CreateConstantBuffer()
    {
        D3D11_BUFFER_DESC constantBufferDesc;
        ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
        constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        constantBufferDesc.ByteWidth = sizeof(m_constantData);
        constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        constantBufferDesc.MiscFlags = 0;
        constantBufferDesc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA constantBufferSubresource;
        ZeroMemory(&constantBufferSubresource, sizeof(D3D11_SUBRESOURCE_DATA));
        constantBufferSubresource.pSysMem = &constantBufferDesc;

        const HRESULT isConstantxBufferCreated =
            WindowManager::GetInstance()->GetDevice()->CreateBuffer(&constantBufferDesc, &constantBufferSubresource, m_constantBuffer.GetAddressOf());

        if (FAILED(isConstantxBufferCreated)) {
            std::cout << __FUNCTION__ << "failed. " << std::hex << isConstantxBufferCreated << std::endl;
        };
    }

    void Object::CreateVertexShader()
    {
        ComPtr<ID3DBlob> shaderBlob;
        ComPtr<ID3DBlob> errorBlob;

        HRESULT isVertexShaderCompiled =
            D3DCompileFromFile(m_vertexShaderFile, 0, 0, "main", "vs_5_0", 0, 0, &shaderBlob, &errorBlob);

        if (FAILED(isVertexShaderCompiled)) 
        {
            if ((isVertexShaderCompiled & D3D11_ERROR_FILE_NOT_FOUND) != 0)
            {
                cout << "File not found." << endl;
            }

            if (errorBlob) 
            {
                cout << "Shader compile error\n" << (char*)errorBlob->GetBufferPointer() << endl;
            }
        }

        WindowManager::GetInstance()->GetDevice()->CreateVertexShader(
            shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
            nullptr, &m_vertexShader);

        vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutElements = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,          D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3,      D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3 * 2,  D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 4 * 3 * 3,  D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        WindowManager::GetInstance()->GetDevice()->CreateInputLayout(
            inputLayoutElements.data(), UINT(inputLayoutElements.size()),
            shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &m_inputLayout);
    }

    void Object::CreatePixelShader()
    {
        ComPtr<ID3DBlob> shaderBlob;
        ComPtr<ID3DBlob> errorBlob;

        HRESULT isPixelShaderCompiled =
            D3DCompileFromFile(m_pixelShaderFile, 0, 0, "main", "ps_5_0", 0, 0, &shaderBlob, &errorBlob);

        if (FAILED(isPixelShaderCompiled)) {
            // 파일이 없을 경우
            if ((isPixelShaderCompiled & D3D11_ERROR_FILE_NOT_FOUND) != 0) {
                cout << "File not found." << endl;
            }

            // 에러 메시지가 있으면 출력
            if (errorBlob) {
                cout << "Shader compile error\n" << (char*)errorBlob->GetBufferPointer() << endl;
            }
        }

        WindowManager::GetInstance()->GetDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
            nullptr, &m_pixelShader);
    }
}
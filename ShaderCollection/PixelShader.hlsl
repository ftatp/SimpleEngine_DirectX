struct Material // 48
{
    float3 ambient;
    float pad1;
    float3 diffuse;
    float pad2;
    float3 specular;
    float shininess;
};

cbuffer PixelShaderConstantBuffer : register(b0)
{
    bool useTexture; // 1 but padded to 4
    float3 eyePosition;
	//int lightMode; // 4
	//float padding[3];
    Material material;
	//Light lights[MAX_LIGHTS]; 
};

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

float4 main(PixelShaderInput input) : SV_TARGET
{

    // Use the interpolated vertex color
    return float4(material.ambient, 1.0);
    //return float4(input.color, 1.0);

}

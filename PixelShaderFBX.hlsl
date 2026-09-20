Texture2D diffuseTexture : register(t0);//从t0槽拿贴图
SamplerState samplerState : register(s0);//从s0拿取样器

cbuffer LightBuffer : register(b1)
{
    float3 lightDirection;
    float intensity;

    float3 lightColor;
    float padding;
};

cbuffer CameraBuffer : register(b2)
{
    float3 cameraPosition;//世界坐标
    float padding2;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal   : NORMAL;
    float2 texCoord : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET
{
	float3 N = normalize(input.normal);
    float3 L = normalize(-lightDirection);
    float NdotL = max(dot(N, L), 0.0f);//物体表面法线 N 和光线方向 L 的夹角余弦值



	
    float3 diffuse = lightColor * intensity * NdotL;//漫反射
	//基础色
    float4 albedo = diffuseTexture.Sample(samplerState,input.texCoord);

    return float4(albedo.rgb * diffuse, albedo.a);
	
    //return diffuseTexture.Sample(samplerState, input.texCoord);
}
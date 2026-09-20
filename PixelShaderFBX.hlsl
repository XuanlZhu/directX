Texture2D diffuseTexture : register(t0);//从t0槽拿贴图
SamplerState samplerState : register(s0);//从s0拿取样器

cbuffer LightBuffer : register(b1)
{
    float3 lightDirection;//世界坐标
    float intensity;

    float3 lightColor;
    float padding;
};

cbuffer CameraBuffer : register(b2)
{
    float3 cameraPosition;//世界坐标
    float padding;
};

struct PSInput
{
    float4 position : SV_POSITION;//裁剪空间坐标
    float3 normal   : NORMAL;//世界坐标
    float2 texCoord : TEXCOORD0;
	
	float3 worldPosition : TEXCOORD1;//世界空间位置，给光照计算使用
};

float4 main(PSInput input) : SV_TARGET
{
	float3 N = normalize(input.normal);//法线
    float3 L = normalize(-lightDirection);//指向光源
    float NdotL = max(dot(N, L), 0.0f);//物体表面法线 N 和光线方向 L 的夹角余弦值
	
	float3 V = normalize(cameraPosition - input.worldPosition);//指向摄像机
	float3 R = reflect(-L, N);//反射光方向
	
	float shininess = 32;//32为常见塑料
	float spec = pow(max(dot(V, R), 0.0f), shininess);//shininess为高光集中度
	
	float specularStrength = 1;//高光强度
	//高光
	float3 specular = lightColor * specularStrength * spec;
	
	
	
	//漫反射
    float3 diffuse = lightColor * intensity * NdotL;
	//基础色
    float4 albedo = diffuseTexture.Sample(samplerState,input.texCoord);
	//最终颜色
	float3 finalColor = albedo.rgb * diffuse + specular;

    return float4(finalColor, albedo.a);
	
    //return diffuseTexture.Sample(samplerState, input.texCoord);
}
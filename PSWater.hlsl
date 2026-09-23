Texture2D diffuseTexture : register(t0);//从t0槽拿贴图
Texture2D reflectionTexture : register(t1);//从t1槽拿反射贴图
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
    float4 position : SV_POSITION;//自动透视除法
    float3 normal   : NORMAL;
    float2 texCoord : TEXCOORD0;
	
	float3 worldPosition : TEXCOORD1;//世界空间位置，给光照计算使用
	float4 reflectionPosition : TEXCOORD2;//
};

float4 main(PSInput input) : SV_TARGET
{
	float3 N = normalize(input.normal);
    float3 L = normalize(-lightDirection);
    float NdotL = max(dot(N, L), 0.0f);//物体表面法线 N 和光线方向 L 的夹角余弦值
	
	//-------------------------------------------------------------------
	float3 V = normalize(cameraPosition - input.worldPosition);//指向摄像机
	float3 R = reflect(-L, N);//反射光方向

	float shininess = 32;//32为常见塑料
	float spec = pow(max(dot(V, R), 0.0f), shininess);//shininess为高光集中度

	float specularStrength = 1;//高光强度
	//高光
	float3 specular = lightColor * specularStrength * spec;
	//-------------------------------------------------------------------
	//镜面反射
	// Clip Space → NDC
    float2 uv = input.reflectionPosition.xy / input.reflectionPosition.w;//透视除法
		
	// NDC [-1,1] → UV [0,1]
    uv = uv * 0.5f + 0.5f;
	
	// 根据你的纹理坐标方向决定是否翻转 Y
    //uv.y = 1.0f - uv.y;

    float4 reflection = reflectionTexture.Sample(samplerState,uv);
	
	
	//--------------------------------------------------------------------
	//环境光
	float3 ambientColor = float3(1.0f, 1.0f, 1.0f);
	float ambientIntensity = 0.2f;
	float3 ambient = ambientColor *ambientIntensity;
	//漫反射
    float3 diffuse = lightColor * intensity * NdotL;
	//基础色
    float4 albedo = diffuseTexture.Sample(samplerState,input.texCoord);
	albedo = reflection;
	//最终颜色
	float3 finalColor = albedo.rgb * (diffuse+ambient) + specular;
	
    return float4(finalColor, albedo.a);
}
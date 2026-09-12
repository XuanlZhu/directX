Texture2D diffuseTexture : register(t0);
SamplerState samplerState : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal   : NORMAL;
    float2 texCoord : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET
{
    return diffuseTexture.Sample(samplerState, input.texCoord);
}
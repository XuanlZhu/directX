cbuffer MatrixBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

struct VSInput
{
    float3 position : POSITION;
    float3 normal   : NORMAL;
    float2 texCoord : TEXCOORD0;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 normal   : NORMAL;
    float2 texCoord : TEXCOORD0;
};

VSOutput main(VSInput input)
{
    VSOutput output;

    // 模型空间 -> 世界空间 -> 观察空间 -> 裁剪空间
    float4 worldPosition = mul(float4(input.position, 1.0f), world);
    float4 viewPosition  = mul(worldPosition, view);
    output.position      = mul(viewPosition, projection);

    // 暂时直接传递法线
    output.normal = input.normal;

    // 传递 UV
    output.texCoord = input.texCoord;

    return output;
}
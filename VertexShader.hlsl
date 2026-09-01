struct VS_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
};


struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};


VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;


    // 顶点坐标
    output.position = float4(
        input.position,
        1.0f
    );


    // 传递纹理坐标给 Pixel Shader
    output.uv = input.uv;


    return output;
}
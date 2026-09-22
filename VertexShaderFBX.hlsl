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
	
	float3 worldPosition : TEXCOORD1;// 世界空间位置，给光照计算使用
};
float3x3 Inverse3x3(float3x3 m)
{
    float det = determinant(m);

    if (abs(det) < 1e-6f)
    {
        return float3x3(
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
        );
    }

    float3x3 adj = float3x3(
         m[1][1] * m[2][2] - m[1][2] * m[2][1],
        -m[0][1] * m[2][2] + m[0][2] * m[2][1],
         m[0][1] * m[1][2] - m[0][2] * m[1][1],

        -m[1][0] * m[2][2] + m[1][2] * m[2][0],
         m[0][0] * m[2][2] - m[0][2] * m[2][0],
        -m[0][0] * m[1][2] + m[0][2] * m[1][0],

         m[1][0] * m[2][1] - m[1][1] * m[2][0],
        -m[0][0] * m[2][1] + m[0][1] * m[2][0],
         m[0][0] * m[1][1] - m[0][1] * m[1][0]
    );

    return adj / det;
}



VSOutput main(VSInput input)
{
    VSOutput output;

    // 模型空间 -> 世界空间 -> 观察空间 -> 裁剪空间
    float4 worldPosition = mul(float4(input.position, 1.0f), world);
    float4 viewPosition  = mul(worldPosition, view);
    output.position      = mul(viewPosition, projection);
	
    // 世界矩阵的逆转置矩阵
	float3x3 normalMatrix = Inverse3x3(transpose((float3x3)world));
    // 法线要乘 世界矩阵的逆转置
	output.normal = normalize(mul(input.normal, normalMatrix));
	
    // 传递 UV
    output.texCoord = input.texCoord;
    //世界坐标
    output.worldPosition = mul(float4(input.position, 1.0f), world);

    return output;
}

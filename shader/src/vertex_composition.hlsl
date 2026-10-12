struct VSInput
{
    float2 position : TEXCOORD0;
    float2 uv : TEXCOORD1;
};

struct VSOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.position = float4(input.position * 2, 0, 1);
    output.uv = input.uv;

    return output;
}

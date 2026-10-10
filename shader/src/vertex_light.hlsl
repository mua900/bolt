cbuffer buffer : register(b0, space1) {
    row_major float4x4 ModelViewProjection;
}

struct VSInput {
    float2 position : TEXCOORD0;
    float2 tex_coord : TEXCOORD1;
    float3 light_position : TEXCOORD2;
    float radius : TEXCOORD3;
    float brightness : TEXCOORD4;
    float4 color : TEXCOORD5;
};

struct VSOutput {
    float4 position : SV_POSITION;
    float2 local_position : TEXCOORD0;
    float radius : TEXCOORD1;
    float brightness : TEXCOORD2;
    float4 color : TEXCOORD3;
};

VSOutput main(VSInput input)
{
    float3 position = input.light_position + float3(input.position * input.radius * 2.0, 0);

    VSOutput output;
    output.position = mul(ModelViewProjection, float4(position, 1.0));
    output.local_position = input.position;
    output.radius = input.radius;
    output.brightness = input.brightness;
    output.color = input.color;
    return output;
}

#include "utility.hlsli"

// @todo a better way to do this
// a common include between shaders and source or a way to automatically generate shader versions
#define FLAG_FLIP_HORIZONTAL 1
#define FLAG_FLIP_VERTICAL   2

cbuffer buffer : register(b0, space1) {
    row_major float4x4 ModelViewProjection;
}

struct VSInput {
    float2 position : TEXCOORD0;
    float2 uv : TEXCOORD1;
    float3 instance_position : TEXCOORD2;
    uint2 rotation_and_flags : TEXCOORD3;
    float2 scale : TEXCOORD4;
    float4 color : TEXCOORD5;
    float2 sourceOffset : TEXCOORD6;
    float2 sourceScale : TEXCOORD7;
};

struct VSOutput {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : TEXCOORD1;
};

VSOutput main(VSInput input)
{
    float rotation = unpack_rotation(input.rotation_and_flags.x);
    uint flags = input.rotation_and_flags.y;

    float2 uv = input.uv;
    if (flags & FLAG_FLIP_HORIZONTAL)
    {
        uv.x = 1.0 - uv.x;
    }
    if (flags & FLAG_FLIP_VERTICAL)
    {
        uv.y = 1.0 - uv.y;
    }
    uv = input.uv * input.sourceScale + input.sourceOffset;

    float2 scale = input.scale * 256;

    VSOutput output;
    float2 p = input.position;
    p.x *= scale.x;
    p.y *= scale.y;
    float c = cos(rotation);
    float s = sin(rotation);
    float2 pos = input.instance_position.xy + float2(p.x * c - p.y * s, p.x * s + p.y * c);

    output.position = mul(ModelViewProjection, float4(pos, input.instance_position.z, 1.0));
    output.uv = uv;
    output.color = input.color;
    return output;
}

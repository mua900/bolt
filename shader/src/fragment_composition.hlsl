Texture2D colorTexture : register(t0, space2);
Texture2D lightTexture : register(t1, space2);
SamplerState colorSampler : register(s0, space2);
SamplerState lightSampler : register(s1, space2);

cbuffer Composition : register(b0, space3)
{
    float4 ambient;
};

struct PSInput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
};

void main(PSInput input, out float4 fragColor : SV_Target)
{
    float4 albedo = colorTexture.Sample(colorSampler, input.uv);
    float4 light = lightTexture.Sample(lightSampler, input.uv);

    float3 color = albedo.rgb * (light.rgb + ambient.rgb);
    fragColor = float4(color, albedo.a);
}

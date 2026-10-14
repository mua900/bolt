struct PSInput {
    float4 position : SV_POSITION;
    float2 local_position : TEXCOORD0;
    float radius : TEXCOORD1;
    float brightness : TEXCOORD2;
    float4 color : TEXCOORD3;
};

void main(PSInput input, out float4 fragColor : SV_Target)
{
    float distance = length(input.local_position * 2.0);

    clip(1.0 - distance);

    float falloff = saturate(1.0 - distance);
    falloff = pow(falloff, 1.4);
    float4 color = float4(input.color.rgb * input.brightness, input.color.a) * falloff;
    fragColor = color;
}

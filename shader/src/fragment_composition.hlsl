struct PSInput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
};

void main(PSInput input, out float4 fragColor : SV_Target)
{
    fragColor = float4(1,1,1,1);
}
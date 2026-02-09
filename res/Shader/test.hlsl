Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

struct VertexIn
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD;
};

struct VertexOut
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    vout.Position = float4(vin.Position, 1.0f);
    vout.TexCoord = vin.TexCoord;
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return gTexture.Sample(gSampler, pin.TexCoord);
}
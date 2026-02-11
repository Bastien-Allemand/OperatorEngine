cbuffer cb : register(b0)
{
    float4x4 gWorldViewProj;
};

struct VertexIn
{
    float3 pos    : POSITION;
    float3 normal : NORMAL;  
    float2 uv     : TEXCOORD;
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    vout.pos = float4(vin.pos, 1.0f); // On ignore la matrice !
    vout.color = float4(1.0f, 1.0f, 0.0f, 1.0f); // Jaune fluo pour être sûr
    return vout;
}
float4 PS(VertexOut pin) : SV_Target
{
    return pin.color;
}
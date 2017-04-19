
struct VTOP
{
	float4 PosNew		: SV_POSITION;
	float4 PosOld		: OLD_POSITION;
	float2 TextureUV	: TUV;
};


Texture2D textureFromFile : register(t0);
SamplerState samplerState : register(s0);

float4 main(VTOP input) : SV_Target
{
	return textureFromFile.Sample(samplerState, input.TextureUV);
	//return float4(0.0f,1.0f,1.0f,1.0f);
}

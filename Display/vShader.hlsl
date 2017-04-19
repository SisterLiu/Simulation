/*
struct VTOP
{
	float4 PosNew		: SV_POSITION;
	float4 PosOld		: OLD_POSITION;
	float2 TextureUV	: TUV;
};

cbuffer ConstantBuffer : register(b0)
{
	float3 PositionChange;
}

VTOP main( float4 pos : POSITION, float2  textureUV: TEXTURE)
{
	VTOP result;
	result.PosOld = pos;
	result.PosNew = pos;
	//result.PosNew = result.PosOld + float4(PositionChange.xy, 0.0f, 0.0f);
	if(PositionChange.z < 0.5f)
	{
		result.TextureUV = textureUV;
	}
	else if(PositionChange.z < 1.5f)
	{
		result.TextureUV = float2(textureUV.y, 1.0f - textureUV.x);
	}
	else if(PositionChange.z < 2.5f)
	{
		result.TextureUV = float2(1.0f - textureUV.x, 1.0f - textureUV.y);
	}
	else
	{
		result.TextureUV = float2(1.0f - textureUV.y, textureUV.x);
	}
	return result;
}
*/
float4 main(float4 pos : POSITION) : SV_POSITION
{
	return pos;
}
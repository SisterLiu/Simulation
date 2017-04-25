
struct VTOP
{
	float4 PosNew		: SV_POSITION;
	float4 PosOld		: OLD_POSITION;
	float2 TextureUV	: TUV;
};

cbuffer ConstantBuffer : register(b0)
{
	float4 PositionChange;
	float2 ScreenSize;
}

VTOP main( float4 pos : POSITION, float2  textureUV: TEXTURE)
{
	VTOP result;
	result.PosOld = pos;
	result.PosNew = pos;
	result.PosNew.x = (pos.x * PositionChange.w + PositionChange.x*2 - ScreenSize.x) / ScreenSize.x;
	result.PosNew.y = -(pos.y * PositionChange.w + PositionChange.y*2 - ScreenSize.y) / ScreenSize.y;
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


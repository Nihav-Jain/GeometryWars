//Texture2D ColorTexture;

//SamplerState ColorSampler;

struct VS_OUTPUT
{
	float4 Position: SV_Position;
	float2 TextureCoordinate : TEXCOORD;
};

float4 main(VS_OUTPUT IN) : SV_TARGET
{
	return float4(0,0.5,1,1);//ColorTexture.Sample(ColorSampler, IN.TextureCoordinate);
}
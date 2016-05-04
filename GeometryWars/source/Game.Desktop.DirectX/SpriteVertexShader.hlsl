cbuffer CBufferPerObject
{
	float4x4 ModelViewProjection : WORLDVIEWPROJECTION;
}

struct VS_INPUT
{
	float2 ObjectPosition: POSITION;
	float2 TextureCoordinate : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 Position: SV_Position;
	float2 TextureCoordinate : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	OUT.Position = mul(float4(IN.ObjectPosition.xy, -1, 1), ModelViewProjection);//float4x4(0.01, 0, 0, 0,  0, 0.01, 0, 0,  0,0,1,0,  0,0,0,1));
	//OUT.Position.w = 1;
	OUT.TextureCoordinate = IN.TextureCoordinate;

	return OUT;
}

struct VS_INPUT
{
	float4 Position: POSITION;
	float4 Color : COLOR;
};

struct VS_OUTPUT
{
	float4 Position: SV_Position;
	float4 Color : COLOR;
};

VS_OUTPUT main(VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	OUT.Position = IN.Position;
	OUT.Color = IN.Color;

	return OUT;
}
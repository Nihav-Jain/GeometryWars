struct VS_INPUT
{
	float4 Position : POSITION;
};

struct VS_OUTPUT
{
	float4 Position: SV_Position;
};

VS_OUTPUT main(VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	OUT.Position = float4(IN.Position.xy, 0, 1);

	return OUT;
}
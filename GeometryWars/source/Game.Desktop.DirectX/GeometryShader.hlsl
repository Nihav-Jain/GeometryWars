cbuffer CVertexBufferPerObject
{
	float4x4 ModelViewProjection : WORLDVIEWPROJECTION;
	float width;
	float3 bullshit;
};

struct GS_INOUT
{
	float4 Position : SV_POSITION;
};
//
GS_INOUT init_inout(float4 Position)
{
	GS_INOUT temp;
	temp.Position = Position;
	return temp;
}

[maxvertexcount(6)]
void main(
	line GS_INOUT input[2],
	inout TriangleStream< GS_INOUT > output
)
{
	//float half_width = 0.01;
	float4 perpendicular = normalize(input[0].Position - input[1].Position).yxzw;
	perpendicular.x = -perpendicular.x;
	float4 p1 = mul(input[0].Position + (0.01f * width * perpendicular), ModelViewProjection);
	float4 p2 = mul(input[0].Position - (0.01f * width * perpendicular), ModelViewProjection);
	float4 p3 = mul(input[1].Position - (0.01f * width * perpendicular), ModelViewProjection);
	float4 p4 = mul(input[1].Position + (0.01f * width * perpendicular), ModelViewProjection);

	output.Append( init_inout(p1) );
	output.Append( init_inout(p2) );
	output.Append( init_inout(p3) );
	output.Append( init_inout(p4) );
	output.Append( init_inout(p3) );
	output.Append( init_inout(p1) );
}
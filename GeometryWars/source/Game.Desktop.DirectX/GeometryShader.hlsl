cbuffer CGeomBufferPerObject
{
	float half_width;
	float3 bullshit;
};

struct GS_INOUT
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

GS_INOUT init_inout(float4 Position, float4 Color)
{
	GS_INOUT temp;
	temp.Position = Position;
	temp.Color = Color;
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
	float4 p1 = input[0].Position + (half_width * perpendicular);
	float4 p2 = input[0].Position - (half_width * perpendicular);
	float4 p3 = input[1].Position - (half_width * perpendicular);
	float4 p4 = input[1].Position + (half_width * perpendicular);
	//output.Append(init_inout(input[0].Position, input[0].Color));
	//output.Append(init_inout(input[1].Position, input[1].Color));
	output.Append(init_inout( p1, input[0].Color ));
	output.Append(init_inout( p2, input[0].Color ));
	output.Append(init_inout( p3, input[1].Color ));
	output.Append(init_inout( p4, input[1].Color ));
	output.Append(init_inout( p3, input[1].Color ));
	output.Append(init_inout( p1, input[0].Color ));
}
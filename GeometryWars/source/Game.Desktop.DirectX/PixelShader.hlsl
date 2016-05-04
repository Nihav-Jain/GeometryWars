cbuffer CGeomBufferPerObject
{
	float4 Color : COLOR;
};

float4 main() : SV_TARGET
{
	return Color;
}
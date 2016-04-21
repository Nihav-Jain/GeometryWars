#pragma once

struct ID3D11Device1		 ;
struct ID3D11DeviceContext;
struct ID3D11VertexShader ;
struct ID3D11PixelShader	 ;
struct ID3D11InputLayout	 ;
struct ID3D11Buffer		 ;

class PolygonRenderer
{
public:
	PolygonRenderer(ID3D11Device1& device, ID3D11DeviceContext& context);
	~PolygonRenderer();
	void Draw();


private:
	struct VertexPositionColor
	{
		XMFLOAT4 Position;
		XMFLOAT4 Color;
	};
	struct CBufferPerObject
	{
		XMFLOAT4X4 WorldViewProjection;
	};

	//struct Co mConstantBuffer
	ID3D11Device1* m_device;
	ID3D11DeviceContext* m_context;
	ID3D11VertexShader* m_vertex_shader;
	ID3D11PixelShader* m_pixel_shader;
	ID3D11InputLayout* m_input_layout;
	ID3D11Buffer* m_vertex_buffer;
	ID3D11Buffer* mConstantBuffer;

	//CBufferPerObject mCBufferPerObject;
	XMFLOAT4X4 mWorldMatrix;
	
};


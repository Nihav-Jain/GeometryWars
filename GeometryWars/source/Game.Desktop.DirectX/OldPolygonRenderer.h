#pragma once

struct ID3D11Device1		 ;
struct ID3D11DeviceContext;
struct ID3D11VertexShader ;
struct ID3D11PixelShader	 ;
struct ID3D11InputLayout	 ;
struct ID3D11Buffer		 ;

class OldPolygonRenderer
{
public:
	OldPolygonRenderer(ID3D11Device1& device, ID3D11DeviceContext& context);
	~OldPolygonRenderer();
	void Draw();


private:
	struct VertexPositionColor
	{
		XMFLOAT4 Position;
		XMFLOAT4 Color;
	};
	struct CVertexBufferPerObject
	{
		XMFLOAT4X4 WorldViewProjection;
	};
	struct CGeometryBufferPerObject
	{
		float width;
		XMFLOAT3 bullshit;
	};

	ID3D11Device1* mDevice;
	ID3D11DeviceContext* mContext;
	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11GeometryShader* mGeomShader;
	ID3D11InputLayout* mInputLayout;
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	ID3D11Buffer* mConstantVertexBuffer;
	ID3D11Buffer* mConstantGeometryBuffer;

	//CBufferPerObject mCBufferPerObject;
	XMFLOAT4X4 mWorldMatrix;
	
};


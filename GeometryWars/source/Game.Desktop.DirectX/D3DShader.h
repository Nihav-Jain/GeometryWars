#pragma once
#include <Shader.h>

struct ID3D11Device1;
struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;
namespace Library
{
	class D3DShader : public Library::Shader
	{
	public:
		D3DShader(ID3D11Device1& device, ID3D11DeviceContext& context);
		virtual ~D3DShader();

		virtual void Init(const std::string & vPath, const std::string & fPath, const std::string & gPath) override;
		virtual void Use() override;
		virtual void SetMatrix4(const std::string & name, const glm::mat4 & value) override;
		virtual void SetVector4(const std::string & name, const glm::vec4 & value) override;
		virtual void SetFloat(const std::string & name, float value) override;
		virtual void SetBool(const std::string & name, bool value) { UNREFERENCED_PARAMETER(name); UNREFERENCED_PARAMETER(value); };
		virtual void SetInt(const std::string & name, std::int32_t value) { UNREFERENCED_PARAMETER(name); UNREFERENCED_PARAMETER(value); };
	private:

		struct CGeometryBufferPerObject
		{
			XMFLOAT4X4 WorldViewProjection;
			float width;
			XMFLOAT3 bullshit;
		};
		struct CPixelBufferPerObject
		{
			XMFLOAT4 Color;
		};

		ID3D11Device1* mDevice;
		ID3D11DeviceContext* mContext;
		ID3D11VertexShader* mVertexShader;
		ID3D11GeometryShader* mGeometryShader;
		ID3D11PixelShader* mPixelShader;

		ID3D11Buffer* mConstantGeometryBuffer;
		ID3D11Buffer* mConstantPixelBuffer;
		CGeometryBufferPerObject mGeoBufferInstance;

		ID3D11InputLayout* mInputLayout;
	};
}
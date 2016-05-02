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

		virtual void Init(const std::string & vPath, const std::string & fPath) override;
		virtual void Use() override;
		virtual void SetMatrix4(const std::string & name, const glm::mat4 & value) override;
		virtual void SetVector4(const std::string & name, const glm::vec4 & value) override;
	private:

		ID3D11Device1* mDevice;
		ID3D11DeviceContext* mContext;
		ID3D11VertexShader* mVertexShader;
		ID3D11PixelShader* mPixelShader;
	};
}
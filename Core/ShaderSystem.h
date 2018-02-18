#pragma once
//셧다운 만들것
class ShaderSystem
{
public:
	ShaderSystem(ID3D11Device * device, ID3D11DeviceContext * deviceContext);
	~ShaderSystem();
	bool Initialize();
	void ShutDown();
	void Render( int type);

private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;

	};


	ID3D11Device * m_device = nullptr;
	ID3D11DeviceContext * m_deviceContext = nullptr;

	ID3D11VertexShader* m_colorVertexShader = nullptr;
	ID3D11PixelShader* m_colorPixelShader = nullptr;
	ID3D11InputLayout* m_colorShaderLayout = nullptr;


	ID3D11Buffer* m_matrixBuffer = nullptr;
};


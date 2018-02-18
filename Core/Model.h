#pragma once
class Model
{
public:
	Model();
	~Model();
	bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* filename, ID3D11ShaderResourceView** pSRV);
	virtual void Render(float frameTime, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix) = 0;

public:
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 scale = { 0,0,0 };
};


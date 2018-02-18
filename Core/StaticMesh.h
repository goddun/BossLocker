#pragma once
#include"Model.h"
#include"ShaderSystem.h"
///////////////**************new**************////////////////////
//Create material structure
struct SurfaceMaterial
{
	std::wstring matName;
	XMFLOAT4 difColor;
	int texArrayIndex;
	bool hasTexture;
	bool transparent;
};

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}
	XMFLOAT3 pos;
	float range;
	XMFLOAT3 dir;
	float cone;
	XMFLOAT3 att;
	float pad2;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};

//Create effects constant buffer's structure//
struct cbPerObject
{
	XMMATRIX  WVP;
	XMMATRIX World;

	///////////////**************new**************////////////////////
	//These will be used for the pixel shader
	XMFLOAT4 difColor;
	bool hasTexture;
	///////////////**************new**************////////////////////
};

struct cbPerFrame
{
	Light  light;
};

class StaticMesh:public Model
{
public:
	StaticMesh(ID3D11Device * device, ID3D11DeviceContext * deviceContext,ShaderSystem* shaderSystem);
	~StaticMesh();

	bool LoadMesh(const std::string& filename,
		bool isRHCoordSys,
		bool computeNormals);
	void Shutdown();
	void Render(float frameTime, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
private:
	void Initialize();

	ID3D11BlendState* Transparency;
	//Mesh variables. Each loaded mesh will need its own set of these
	XMMATRIX meshWorld;
	int meshSubsets = 0;


	std::vector<int> subsetIndexStart;
	std::vector<int> subsetMaterialArray;
	std::vector<SurfaceMaterial> material;



	//텍스쳐
	std::vector<ID3D11ShaderResourceView*> meshSRV;
	std::vector<std::wstring> textureNameArray;
	ID3D11SamplerState* m_sampleState = nullptr;
	//버퍼
	ID3D11Buffer* m_vertexBuffer=nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	cbPerObject cbPerObj;
	cbPerFrame constbuffPerFrame;
	ID3D11Buffer* cbPerFrameBuffer;
	ID3D11Buffer* cbPerObjectBuffer;
	XMMATRIX WVP;
	Light light;
	//다이렉트
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;
	//쉐이더
	ShaderSystem* m_shaderSystem = nullptr;
};


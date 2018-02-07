#pragma once
#include <map>
#include <vector>
#include <assimp\scene.h>      // C++ importer interface
#include <assimp\Importer.hpp>       // Output data structure
#include<assimp\postprocess.h>
#include"lightshaderclass.h"
#include"textureclass.h"

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

struct Vertex
{
	XMFLOAT3 m_pos;
	XMFLOAT2 m_tex;
	XMFLOAT3 m_normal;

	Vertex() {}

	Vertex(const XMFLOAT3& pos, const XMFLOAT2& tex, const XMFLOAT3& normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
};


class Mesh
{
public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, LightShaderClass* shader);
	~Mesh();

	bool LoadMesh(const std::string& Filename);
	void Render(XMMATRIX&, XMMATRIX&, XMMATRIX&, ID3D11ShaderResourceView*);
	int GetIndex()
	{
		return m_Entries[0].NumIndices;
	}
private:
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	void Clear();

	struct MeshEntry {
		MeshEntry();

		~MeshEntry();

		void Init(ID3D11Device* device , const std::vector<Vertex>& Vertices,
			const std::vector<unsigned int>& Indices);


		unsigned int NumIndices;

		ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	};

	Assimp::Importer m_Importer;
	const aiScene* m_pScene;
	std::vector<MeshEntry> m_Entries;

	XMMATRIX m_GlobalInverseTransform;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	LightShaderClass* m_shader;
public:
	XMFLOAT3 position;
	XMFLOAT4 rotation;
	XMFLOAT3 scale;
};


#pragma once

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
	unsigned int m_boneID[4] = {0,};
	float m_weight[4] = {0,};

	
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
	void BoneTransform(float TimeInSeconds, vector<XMMATRIX>& Transforms);

	void Render(float,XMMATRIX&, XMMATRIX&, XMMATRIX&, ID3D11ShaderResourceView*);
	int GetIndex()
	{
		return m_Entries[0].NumIndices;
	}

private:
	struct BoneInfo
	{
		XMMATRIX BoneOffset = XMMatrixIdentity();
		XMMATRIX FinalTransformation = XMMatrixIdentity();
	};

	struct VertexBoneData
	{
		uint IDs[NUM_BONES_PER_VEREX];
		float Weights[NUM_BONES_PER_VEREX];

		VertexBoneData()
		{
			Reset();
		};

		void Reset()
		{
			ZERO_MEM(IDs);
			ZERO_MEM(Weights);
		}

		void AddBoneData(uint BoneID, float Weight);
	};

	struct MeshEntry {
		MeshEntry();

		~MeshEntry();

		void Init(ID3D11Device* device , const std::vector<Vertex>& Vertices,
			const std::vector<unsigned int>& Indices);


		unsigned int NumIndices;

		vector<VertexBoneData> BoneData;
		ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;

	
	};
private:
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	void LoadBones(uint MeshIndex, const aiMesh* pMesh);
	void Clear();

private:
	//ASSIMP IMPORT
	Assimp::Importer m_Importer;
	const aiScene* m_pScene;
	//MESH
	std::vector<MeshEntry> m_Entries;
	//ANIMATION
	XMMATRIX m_GlobalInverseTransform;
	vector<aiNode*>ai_nodes;
	vector<aiNodeAnim*>ai_nodes_anim;
	map<string, uint> m_BoneMapping; // maps a bone name to its index
	vector<BoneInfo> m_BoneInfo;
	uint m_NumBones = 0;

	void recursiveNodeProcess(aiNode* node);
	void AnimNodeProcess();
	//���̷�Ʈ
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	LightShaderClass* m_shader;
public:
	XMFLOAT3 position;
	XMFLOAT4 rotation;
	XMFLOAT3 scale;
};


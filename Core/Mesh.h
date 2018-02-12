#pragma once

#include <assimp\scene.h>      // C++ importer interface
#include <assimp\Importer.hpp>       // Output data structure
#include<assimp\postprocess.h>
#include"lightshaderclass.h"
#include"textureclass.h"
#include"Model.h"

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

struct AnimationVertex
{
	XMFLOAT3 m_pos;
	XMFLOAT2 m_tex;
	XMFLOAT3 m_normal;
	unsigned int m_boneID[4] = {0,};
	float m_weight[4] = {0,};

	
	AnimationVertex() {}

	AnimationVertex(const XMFLOAT3& pos, const XMFLOAT2& tex, const XMFLOAT3& normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
};


class Mesh:public Model
{
public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, LightShaderClass* shader);
	~Mesh();

	bool LoadMesh(const std::string& Filename);
	void BoneTransform(float TimeInSeconds, vector<XMMATRIX>& Transforms);

	void Render(float,XMMATRIX&, XMMATRIX&, XMMATRIX&);
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

		void Init(ID3D11Device* device , const std::vector<AnimationVertex>& Vertices,
			const std::vector<unsigned int>& Indices);


		unsigned int NumIndices=0;
		unsigned int BaseVertex=0;
		unsigned int BaseIndex=0;
		unsigned int MaterialIndex=0;

		vector<VertexBoneData> BoneData;
		ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;

	
	};
private:
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	void LoadBones(unsigned int MeshIndex, const aiMesh* pMesh, vector<VertexBoneData>& Bones);
	void Clear();

	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const XMMATRIX& ParentTransform);
	//보간 함수들
	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	//노드에 값 검색
	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
	uint FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	uint FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	uint FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
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


	//다이렉트
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	LightShaderClass* m_shader;

};


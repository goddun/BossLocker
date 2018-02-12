#include"pch.h"
#include "Mesh.h"


#define INVALID_OGL_VALUE 0xffffffff


void MatrixConvert(const aiMatrix4x4& param, XMMATRIX& out)
{
	XMFLOAT4X4 Data;
	Data._11 = param.a1; Data._12 = param.a2; Data._13 = param.a3; Data._14 = param.a4;
	Data._21 = param.b1; Data._22 = param.b2; Data._23 = param.b3; Data._24 = param.b4;
	Data._31 = param.c1; Data._32 = param.c2; Data._33 = param.c3; Data._34 = param.c4;
	Data._41 = param.d1; Data._42 = param.d2; Data._43 = param.d3; Data._44 = param.d4;

	out = XMLoadFloat4x4(&Data);
}

void MatrixConvert(const aiMatrix3x3& param, XMMATRIX& out)
{
	XMFLOAT4X4 Data;
	Data._11 = param.a1; Data._12 = param.a2; Data._13 = param.a3; Data._14 = 0.0f;
	Data._21 = param.b1; Data._22 = param.b2; Data._23 = param.b3; Data._24 = 0.0f;
	Data._31 = param.c1; Data._32 = param.c2; Data._33 = param.c3; Data._34 = 0.0f;
	Data._41 = 0.0f;     Data._42 = 0.0f;     Data._43 = 0.0f;	   Data._44 = 1.0f;

	out = XMLoadFloat4x4(&Data);
}

Mesh::MeshEntry::MeshEntry()
{
	NumIndices = 0;
	m_indexBuffer = NULL;
	m_vertexBuffer = NULL;
};

Mesh::MeshEntry::~MeshEntry()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}


void Mesh::MeshEntry::Init(ID3D11Device* device, const std::vector<AnimationVertex>& Vertices,
	const std::vector<unsigned int>& Indices)
{
	NumIndices = Indices.size();


	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * Vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = &(Vertices[0]);
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);


	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * NumIndices;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = &(Indices[0]);
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

}



Mesh::Mesh(ID3D11Device * device, ID3D11DeviceContext * deviceContext, LightShaderClass * shader)
{
	m_device = device;
	m_deviceContext = deviceContext;
	m_shader = shader;
	m_pScene = nullptr;
}

Mesh::~Mesh()
{
	Clear();
}



bool Mesh::LoadMesh(const std::string& Filename)
{
	// Release the previously loaded mesh (if it exists)
	Clear();

	bool Ret = false;
	

	m_pScene = m_Importer.ReadFile(Filename.c_str(), ASSIMP_LOAD_FLAGS);

	if (m_pScene) {
		MatrixConvert( m_pScene->mRootNode->mTransformation, m_GlobalInverseTransform);
		XMVECTOR det = XMMatrixDeterminant(m_GlobalInverseTransform);
		m_GlobalInverseTransform = XMMatrixInverse(&det , m_GlobalInverseTransform);//루트 노드의 역행렬 계산.
		
		Ret = InitFromScene(m_pScene, Filename);
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), m_Importer.GetErrorString());
	}

	return Ret;
}

bool Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
	m_Entries.resize(pScene->mNumMeshes);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh);
	}

	return true;
}

void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
	int boneArraysSize = paiMesh->mNumVertices*NUM_BONES_PER_VEREX;

	std::vector<AnimationVertex> Verticies;
	std::vector<unsigned int> Indices;
	vector<VertexBoneData> Bones;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		AnimationVertex v(XMFLOAT3(pPos->x, pPos->y, pPos->z),
			XMFLOAT2(pTexCoord->x, pTexCoord->y),
			XMFLOAT3(pNormal->x, pNormal->y, pNormal->z));

		Verticies.push_back(v);
	}
	//LoadBones(Index, paiMesh, Bones);

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	vector<int> boneIDs;
	boneIDs.resize(boneArraysSize);
	vector<float> boneWeights;
	boneWeights.resize(boneArraysSize);

	int boneCount = paiMesh->mNumBones;
	for (int i = 0; i < boneCount; i++)
	{
		aiBone* aiBone = paiMesh->mBones[i];//뼈에 대한 포인터를 얻어와서
//뼈에 대한 데이터 구조를 만든다.
		uint BoneIndex = 0;
		string BoneName(paiMesh->mBones[i]->mName.data);

		if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
			BoneIndex = m_NumBones;
			m_NumBones++;
			BoneInfo bi;
			m_BoneInfo.push_back(bi);
		}
		else {
			BoneIndex = m_BoneMapping[BoneName];
		}

		m_BoneMapping[BoneName] = BoneIndex;
		MatrixConvert(paiMesh->mBones[i]->mOffsetMatrix, m_BoneInfo[BoneIndex].BoneOffset);
//뼈 무게, 아이디 -> 버택스별 데이터 넣기, 쉐이더에 전달할 데이터임
		int weightCount = aiBone->mNumWeights;//무게값 넣기
		for (int j = 0; j < weightCount; j++)
		{
			aiVertexWeight weight = aiBone->mWeights[j];

			unsigned int vertexStart = weight.mVertexId*NUM_BONES_PER_VEREX;
			for (int k = 0; k < NUM_BONES_PER_VEREX; k++)
			{
				if (boneWeights.at(vertexStart + k) == 0)
				{
					boneWeights.at(vertexStart + k) = weight.mWeight;
					boneIDs.at(vertexStart + k) = i;

					
					Verticies.at(weight.mVertexId).m_boneID[k] = i;
					Verticies.at(weight.mVertexId).m_weight[k] = weight.mWeight;

					break;
				}
			}
		}
	}


	m_Entries[Index].Init(m_device,Verticies, Indices);
}

void Mesh::LoadBones(unsigned int MeshIndex, const aiMesh* pMesh, vector<VertexBoneData>& Bones)
{
	for (uint i = 0; i < pMesh->mNumBones; i++) {
		uint BoneIndex = 0;
		string BoneName(pMesh->mBones[i]->mName.data);

		if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
			// Allocate an index for a new bone
			BoneIndex = m_NumBones;
			m_NumBones++;
			BoneInfo bi;
			m_BoneInfo.push_back(bi);
			MatrixConvert(pMesh->mBones[i]->mOffsetMatrix, m_BoneInfo[BoneIndex].BoneOffset);
			m_BoneMapping[BoneName] = BoneIndex;
		}
		else {
			BoneIndex = m_BoneMapping[BoneName];
		}

		for (uint j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
			uint VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
			float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
			Bones[VertexID].AddBoneData(BoneIndex, Weight);
		}
	}
}

void Mesh::Clear()
{
	int size = m_Entries.size();
	for (unsigned int i = 0; i <size ; i++) {
		if (m_Entries[i].m_indexBuffer)
		{
			m_Entries[i].m_indexBuffer->Release();
			m_Entries[i].m_indexBuffer = nullptr;
		}
		if (m_Entries[i].m_vertexBuffer)
		{
			m_Entries[i].m_vertexBuffer->Release();
			m_Entries[i].m_vertexBuffer = nullptr;
		}
	}
}

void Mesh::Render(float frameTime,XMMATRIX& worldMatrix, XMMATRIX& viewMatrix,XMMATRIX& projectionMatrix)
{

	vector<XMMATRIX> skeleton;

	BoneTransform(frameTime, skeleton);


	for (unsigned int i = 0; i < m_Entries.size(); i++) {

	
		unsigned int stride;
		unsigned int offset;
		// Set vertex buffer stride and offset.
		stride = sizeof(Vertex);
		offset = 0;

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		m_deviceContext->IASetVertexBuffers(0, 1, &m_Entries[i].m_vertexBuffer, &stride, &offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		m_deviceContext->IASetIndexBuffer(m_Entries[i].m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		 XMMATRIX move = XMMatrixTranslation(position.x,position.y,position.z);
		 worldMatrix = XMMatrixMultiply(worldMatrix, move);

		 XMVECTOR rotate = XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
		 worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationQuaternion(rotate));

		//m_shader->Render(m_deviceContext,m_Entries[i].NumIndices, worldMatrix, viewMatrix, projectionMatrix, texture, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f },skeleton);
	}

}

void Mesh::VertexBoneData::AddBoneData(uint BoneID, float Weight)
{
	for (uint i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); i++) {
		if (Weights[i] == 0.0) {
			IDs[i] = BoneID;
			Weights[i] = Weight;
			return;
		}
	}

	// should never get here - more bones than we have space for
	assert(0);
}

void Mesh::BoneTransform(float TimeInSeconds, vector<XMMATRIX>& Transforms)
{
	XMMATRIX Identity = XMMatrixIdentity();
	TimeInSeconds *= 100;
	float TicksPerSecond = m_pScene->mAnimations[0]->mTicksPerSecond != 0 ?
		m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f;
	float TimeInTicks = TimeInSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, m_pScene->mAnimations[0]->mDuration);

	ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);

	Transforms.resize(m_NumBones);

	for (uint i = 0; i < m_NumBones; i++) {
		XMMATRIX temp = XMMatrixTranspose(m_BoneInfo[i].FinalTransformation);
		Transforms[i] = temp;
		//Transforms[i] = m_BoneInfo[i].FinalTransformation;
	}
}

void Mesh::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const XMMATRIX& ParentTransform)
{
	string NodeName(pNode->mName.data);

	const aiAnimation* pAnimation = m_pScene->mAnimations[0];

	XMMATRIX NodeTransformation;
	MatrixConvert(pNode->mTransformation, NodeTransformation);

	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim) {
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		XMMATRIX ScalingM = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		XMMATRIX RotationM = XMMatrixIdentity();
		MatrixConvert(RotationQ.GetMatrix(), RotationM);
			

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		XMMATRIX TranslationM = XMMatrixTranslation(Translation.x, Translation.y, Translation.z);

		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	XMMATRIX GlobalTransformation = ParentTransform * NodeTransformation;
	
	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
		uint BoneIndex = m_BoneMapping[NodeName];
		m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
	}

	for (uint i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}
}



void Mesh::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	uint ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	uint NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void Mesh::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	uint RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	uint NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}

void Mesh::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	uint PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	uint NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

#pragma region 노드에서 원하는것들을 찾는 함수들
const aiNodeAnim* Mesh::FindNodeAnim(const aiAnimation* pAnimation, const string NodeName)
{
	for (uint i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (string(pNodeAnim->mNodeName.data) == NodeName) {
			return pNodeAnim;
		}
	}

	return NULL;
}
uint Mesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (uint i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}


uint Mesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (uint i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}


uint Mesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (uint i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}
#pragma endregion
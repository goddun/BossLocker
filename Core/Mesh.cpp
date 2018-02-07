#include"pch.h"
#include "Mesh.h"


#define INVALID_OGL_VALUE 0xffffffff

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


void Mesh::MeshEntry::Init(ID3D11Device* device, const std::vector<Vertex>& Vertices,
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


Mesh::Mesh(ID3D11Device * device, ID3D11DeviceContext* deviceContext, LightShaderClass* shader)
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

void MatrixConvert(const aiMatrix4x4& param, XMMATRIX& out)
{

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
		;
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

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Vertex v(XMFLOAT3(pPos->x, pPos->y, pPos->z),
			XMFLOAT2(pTexCoord->x, pTexCoord->y),
			XMFLOAT3(pNormal->x, pNormal->y, pNormal->z));

		Vertices.push_back(v);
	}

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	m_Entries[Index].Init(m_device,Vertices, Indices);
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

void Mesh::Render(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix,XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture)
{
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

		//const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		//if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
		//	m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
		//}

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		 XMMATRIX move = XMMatrixTranslation(0, 0,0);
		 worldMatrix = XMMatrixMultiply(worldMatrix, move);

		m_shader->Render(m_deviceContext,m_Entries[i].NumIndices, worldMatrix, viewMatrix, projectionMatrix, texture, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f });
	}

}

#include"kpu.h"
#include "TextureModel.h"



TextureModel::TextureModel()
{
	
}

TextureModel::TextureModel(const TextureModel &other)
{
}


TextureModel::~TextureModel()
{
}

bool TextureModel::Initialize(ID3D11Device *device, const char* path, const int type)
{
	bool result;
	if (type == FBX)
	{
		result = LoadFBX(path);
		if (!result)
			return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

void TextureModel::GetFbxInfo(FbxNode* Node)
{
	int numKids = Node->GetChildCount();
	FbxNode *childNode = 0;

	for (int i = 0; i<numKids; i++)
	{
		childNode = Node->GetChild(i);
		FbxMesh *mesh = childNode->GetMesh();

		if (mesh != NULL)
		{
			//================= Get Vertices ====================================
			int numVerts = mesh->GetControlPointsCount();

			for (int j = 0; j<numVerts; j++)
			{
				FbxVector4 vert = mesh->GetControlPointAt(j);
				D3DXVECTOR3 vertex;
				vertex.x = (float)vert.mData[0];
				vertex.y = (float)vert.mData[1];
				vertex.z = (float)vert.mData[2];
				
			}
			//================= Get Indices ====================================
			//numIndices = mesh->GetPolygonVertexCount();
			//indices = new int[numIndices];
			//indices = mesh->GetPolygonVertices();
			//cout << numIndices;
			//================= Get Normals ====================================


			FbxGeometryElementNormal* normalEl = mesh->GetElementNormal();
			if (normalEl)
			{
				int numNormals = mesh->GetPolygonCount() * 3;
				float *normals = new float[numNormals * 3];


				int vertexCounter = 0;
				for (int polyCounter = 0; polyCounter<mesh->GetPolygonCount(); polyCounter++)
				{
					for (int i = 0; i<3; i++)
					{
						FbxVector4 normal = normalEl->GetDirectArray().GetAt(vertexCounter);
						normals[vertexCounter * 3 + 0] = normal[0];
						normals[vertexCounter * 3 + 1] = normal[1];
						normals[vertexCounter * 3 + 2] = normal[2];
						cout << "\n" << normals[vertexCounter * 3 + 0] << " " << normals[vertexCounter * 3 + 1] << " " << normals[vertexCounter * 3 + 2];
						vertexCounter++;
					}
				}
			}

		}
		this->GetFbxInfo(childNode);
	}
}

bool TextureModel::LoadFBX(const char* path)
{
		// Change the following filename to a suitable filename value.

	// Initialize the SDK manager. This object handles memory management.
	FbxManager* lSdkManager = FbxManager::Create();
	// Create the IO settings object.
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	// Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// Use the first argument as the filename for the importer.
	if (!lImporter->Initialize(path, -1, lSdkManager->GetIOSettings())) 
	{
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}
	// Create a new scene so that it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

	// Import the contents of the file into the scene.
	lImporter->Import(lScene);

	// The file is imported, so get rid of the importer.
	lImporter->Destroy();

	FbxNode* pFbxRootNode = lScene->GetRootNode();

#pragma region fbxvertex Drawing
	if (pFbxRootNode)//루트 노드가 존재하면
	{
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)//루트 노드의 자식 개수 만큼 반복
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);//첫번째 자식을 얻어온다.

			if (pFbxChildNode->GetNodeAttribute() == NULL)//
				continue;

			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if (AttributeType != FbxNodeAttribute::eMesh)//자식노드의 어트리뷰트가 메시가 아니면 제낀다.
				continue;

			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();//메시면 노드에서 메시 어트리뷰트를 얻어온다.

			FbxVector4* pVertices = pMesh->GetControlPoints();//메시의 제어점들을 얻어와서 행렬에 넣는다.

			FbxGeometryElementNormal* normalEl = pMesh->GetElementNormal();//메시의 노말벡터를 얻어와서 행렬에 넣는다.

			int vertexCounter = 0;

			

			for (int poly = 0; poly < pMesh->GetPolygonCount(); poly++)//메시의 폴리곤 갯수를 구한다. 모든 폴리곤을 한바퀴 돌아본다.
			{
				int iNumVertices = pMesh->GetPolygonSize(poly);//폴리곤 사이즈를 구한다.
				assert(iNumVertices == 3);//폴리곤의 사이즈가 3개 즉 삼각형 일때만 정점을 구한다.
				for (int vert = 0; vert < iNumVertices; vert++) {//3번 돈다.
					int iControlPointIndex = pMesh->GetPolygonVertex(poly, vert);

					D3DXVECTOR3 vertex;
					vertex.x = (float)pVertices[iControlPointIndex].mData[0];
					vertex.y = (float)pVertices[iControlPointIndex].mData[1];
					vertex.z = (float)pVertices[iControlPointIndex].mData[2];

					D3DXVECTOR3 normalVector;

					FbxVector4 normal = normalEl->GetDirectArray().GetAt(vertexCounter);

					normalVector.x =  (float)normal[0];
					normalVector.y = (float)normal[1];
					normalVector.z = (float)normal[2];

					D3DXVECTOR2 uv;

					FbxVector2 fbxTexCoord;
					FbxStringList UVSetNameList;

					

					for (int l = 0; l < pMesh->GetElementUVCount(); ++l)
					{
						FbxGeometryElementUV* leUV = pMesh->GetElementUV(l);
						int lTextureUVIndex = pMesh->GetTextureUVIndex(poly, vert);
						fbxTexCoord = leUV->GetDirectArray().GetAt(lTextureUVIndex);
						// Convert to floats
						uv[0] = static_cast<float>(fbxTexCoord[0]);
						uv[1] = static_cast<float>(fbxTexCoord[1]);
					}

					

					//std::cout << uv[0] << "  " << uv[1] << endl;
					//cout << "\n" << normals[vertexCounter * 3 + 0] << " " << normals[vertexCounter * 3 + 1] << " " << normals[vertexCounter * 3 + 2];
					
					verticies.push_back({ vertex,uv,normalVector });

					vertexCounter++;
				}
			}
		}
	}

#pragma endregion

	lSdkManager->Destroy();


	return true;
}

bool TextureModel::LoadTexture(ID3D11Device *device , WCHAR* path, const int type)
{
	if (type == DDS)
	{
		auto result = m_texture.Load_DDS(device, path);
		if (!result)
			return false;
	}
	return true;
}

void TextureModel::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();
	return;
}

void TextureModel::Render(ID3D11DeviceContext *deviceContext)
{
	RenderBuffers(deviceContext);

	return;
}

int TextureModel::GetIndexCount()
{
	return indicies.size();
}

bool TextureModel::InitializeBuffers(ID3D11Device *device)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;





	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(TextureVertex) * verticies.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = &(verticies[0]);
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//// Set up the description of the static index buffer.
	//indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//indexBufferDesc.ByteWidth = sizeof(unsigned long) * indicies.size();
	//indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//indexBufferDesc.CPUAccessFlags = 0;
	//indexBufferDesc.MiscFlags = 0;
	//indexBufferDesc.StructureByteStride = 0;

	//// Give the subresource structure a pointer to the index data.
	//indexData.pSysMem = &(indicies[0]);
	//indexData.SysMemPitch = 0;
	//indexData.SysMemSlicePitch = 0;

	//// Create the index buffer.
	//result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	//if (FAILED(result))
	//{
	//	return false;
	//}



	return true;
}

void TextureModel::ShutdownBuffers()
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

	return;
}

void TextureModel::RenderBuffers(ID3D11DeviceContext *deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(TextureVertex);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	//deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

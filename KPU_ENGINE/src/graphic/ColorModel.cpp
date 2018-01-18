#include "kpu.h"
#include "ColorModel.h"





//============================================================================================
ColorModel::ColorModel()
{
}


ColorModel::~ColorModel()
{
}

bool ColorModel::Initialize(ID3D11Device *device, const char *path, const int type)
{
	bool result;

	//FBXExporter* myExporter = new FBXExporter();
	//myExporter->Initialize();
	//myExporter->LoadScene("../Asset/wolf.fbx", "../Asset");

	if (type == FBX)
	{
		m_fbxManager = FbxManager::Create();
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




bool ColorModel::LoadFBX(const char *path)
{
	// Change the following filename to a suitable filename value.

	// Initialize the SDK manager. This object handles memory management.

	// Create the IO settings object.
	FbxIOSettings *ios = FbxIOSettings::Create(m_fbxManager, IOSROOT);
	m_fbxManager->SetIOSettings(ios);

	// Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(m_fbxManager, "");

	// Use the first argument as the filename for the importer.
	if (!lImporter->Initialize(path, -1, m_fbxManager->GetIOSettings()))
	{
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}
	// Create a new scene so that it can be populated by the imported file.
	mScene = FbxScene::Create(m_fbxManager, "myScene");

	// Import the contents of the file into the scene.
	lImporter->Import(mScene);

	// The file is imported, so get rid of the importer.
	lImporter->Destroy();

	FbxNode* pFbxRootNode = mScene->GetRootNode();


	

#pragma region fbxvertex Drawing
	if (pFbxRootNode)
	{
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			if (pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if (AttributeType != FbxNodeAttribute::eMesh)
				continue;

			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();

			FbxVector4* pVertices = pMesh->GetControlPoints();

			for (int j = 0; j < pMesh->GetPolygonCount(); j++)
			{
				int iNumVertices = pMesh->GetPolygonSize(j);
				assert(iNumVertices == 3);
				for (int k = 0; k < iNumVertices; k++) {
					int iControlPointIndex = pMesh->GetPolygonVertex(j, k);

					D3DXVECTOR3 vertex;
					vertex.x = (float)pVertices[iControlPointIndex].mData[0];
					vertex.y = (float)pVertices[iControlPointIndex].mData[1];
					vertex.z = (float)pVertices[iControlPointIndex].mData[2];
				
					verticies.push_back({ vertex, D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f) });

					FbxVector4 fbxNorm(0, 0, 0, 0);
					FbxVector2 fbxUV(0, 0);

				
				}
			}
		}
	}

#pragma endregion



	return true;
}

void ColorModel::Shutdown()
{
	if (m_fbxManager)
	{
		// Destroy the SDK manager and all the other objects it was handling.
		m_fbxManager->Destroy();
		m_fbxManager = nullptr;
	}
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();
	return;
}


void ColorModel::Render(ID3D11DeviceContext *deviceContext)
{
	RenderBuffers(deviceContext);

	return;
}



int ColorModel::GetIndexCount()
{
	return indicies.size();
}

bool ColorModel::InitializeBuffers(ID3D11Device *device)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;





	// Set up the description of the static vertex buffer.
	vertexBufferDesc.ByteWidth = sizeof(ColorVertex) * verticies.size();
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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

void ColorModel::ShutdownBuffers()
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

void ColorModel::RenderBuffers(ID3D11DeviceContext *deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(ColorVertex);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//// Set the index buffer to active in the input assembler so it can be rendered.
	//deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

#include "pch.h"
#include "ShaderSystem.h"


ShaderSystem::ShaderSystem(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	m_device = device;
	m_deviceContext = deviceContext;
}


ShaderSystem::~ShaderSystem()
{

}

bool ShaderSystem::Initialize()
{
	//DEBUG
	HRESULT result;
	ID3D10Blob* errorMessage;
	char* compileErrors=nullptr;

	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	
#pragma region COLORSHADER
	result = D3DCompileFromFile(L"../Core/shader/color.vs", NULL, NULL, "VS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
#ifdef DEBUG
		// Get a pointer to the error message text buffer.
		compileErrors = (char*)(errorMessage->GetBufferPointer());
		cout << compileErrors << endl;
#endif // DEBUG
		return false;
	}


	result = D3DCompileFromFile(L"../Core/shader/color.ps", NULL, NULL, "PS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
#ifdef DEBUG
		// Get a pointer to the error message text buffer.
		compileErrors = (char*)(errorMessage->GetBufferPointer());
		cout << compileErrors << endl;
#endif // DEBUG
		return false;
	}

	// Create the vertex shader from the buffer.
	result = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_colorVertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_colorPixelShader);
	if (FAILED(result))
	{
		return false;
	}



	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	numElements = ARRAYSIZE(layout);

	m_device->CreateInputLayout(layout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_colorShaderLayout);
	m_deviceContext->IASetInputLayout(m_colorShaderLayout);
#pragma endregion

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;


	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = m_device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}





	return true;
}

void ShaderSystem::ShutDown()
{
	if (m_colorPixelShader) m_colorPixelShader->Release();
	if (m_colorVertexShader)m_colorVertexShader->Release();
	if (m_colorShaderLayout)m_colorShaderLayout->Release();
}

void ShaderSystem::Render(int type)
{
	switch (type)
	{
	case COLOR_SHADER:
		// Set the vertex input layout.
		m_deviceContext->IASetInputLayout(m_colorShaderLayout);

		// Set the vertex and pixel shaders that will be used to render this triangle.
		m_deviceContext->VSSetShader(m_colorVertexShader, NULL, 0);
		m_deviceContext->PSSetShader(m_colorPixelShader, NULL, 0);


		break;
	}

}

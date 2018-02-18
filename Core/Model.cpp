#include "pch.h"
#include "Model.h"


Model::Model()
{
}


Model::~Model()
{
}

bool Model::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* filename, ID3D11ShaderResourceView** pSRV)
{
	wchar_t ext[_MAX_EXT];
	_wsplitpath_s(filename, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);

	ScratchImage image;
	HRESULT hr;
	if (_wcsicmp(ext, L".dds") == 0)
	{
		hr = LoadFromDDSFile(filename, DDS_FLAGS_NONE, nullptr, image);
	}
	else if (_wcsicmp(ext, L".tga") == 0)
	{
		hr = LoadFromTGAFile(filename, nullptr, image);
	}
	else if (_wcsicmp(ext, L".hdr") == 0)
	{
		hr = LoadFromHDRFile(filename, nullptr, image);
	}
	else
	{
		hr = LoadFromWICFile(filename, WIC_FLAGS_NONE, nullptr, image);
	}

	if (SUCCEEDED(hr))
	{

		hr = CreateShaderResourceView(device,
			image.GetImages(), image.GetImageCount(),
			image.GetMetadata(), pSRV);

		if (FAILED(hr))
		{
			wchar_t buff[2048] = {};
			swprintf_s(buff, L"Failed creating texture from file\n\nFilename = %ls\nHRESULT = %08X", filename, hr);
			MessageBox(nullptr, buff, L"DDSView", MB_OK | MB_ICONEXCLAMATION);
			return false;
		}

		return true;
	}

	return false;
}
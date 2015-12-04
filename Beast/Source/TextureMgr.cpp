#include "TextureMgr.h"
#include <locale>
#include <codecvt>
#include <string>

TextureMgr::TextureMgr() : md3dDevice(0)
{
}

TextureMgr::~TextureMgr()
{
	for(auto it = mTextureSRV.begin(); it != mTextureSRV.end(); ++it)
    {
		RELEASE(it->second);
    }

	mTextureSRV.clear();
}

void TextureMgr::Init(ID3D11Device* device)
{
	md3dDevice = device;
}

ID3D11ShaderResourceView* TextureMgr::CreateTexture(std::wstring filename)
{
	ID3D11ShaderResourceView* srv = 0;

	// Does it already exist?
	if( mTextureSRV.find(filename) != mTextureSRV.end() )
	{
		srv = mTextureSRV[filename];
	}
	else
	{
		HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, filename.c_str(), 0, 0, &srv, 0 ));

		mTextureSRV[filename] = srv;
	}

	return srv;
}
 
ID3D11ShaderResourceView* TextureMgr::CreateTexture(std::string filename)
{
	ID3D11ShaderResourceView* srv = 0;

	std::wstring wide(filename.begin(), filename.end());

	// Does it already exist?
	if (mTextureSRV.find(wide) != mTextureSRV.end())
	{
		srv = mTextureSRV[wide];
	}
	else
	{
		HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, wide.c_str(), 0, 0, &srv, 0));

		mTextureSRV[wide] = srv;
	}

	return srv;
}

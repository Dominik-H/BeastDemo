/*************************************************************************************
* File: Init.h; Date of creation: 12.Feb.2015; Date of Last Modification: 12.Feb.2015
*
* This is a header file to helper stuff.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#include "Texture.h"

Texture::Texture( void )
{
	texture = 0;
}

Texture::Texture( const Texture& text )
{
}

Texture::~Texture( void )
{
	RELEASE(texture);
}

bool Texture::Init( ID3D11Device* device, WCHAR* filename )
{
	HR( D3DX11CreateShaderResourceViewFromFile( device, filename, NULL, NULL, &texture, NULL ) );

	return true;
}

void Texture::Destroy( void )
{
	RELEASE( texture );
}

ID3D11ShaderResourceView* Texture::GetTexture( void )
{
	return texture;
}
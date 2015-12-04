/*************************************************************************************
* File: Init.h; Date of creation: 12.Feb.2015; Date of Last Modification: 12.Feb.2015
*
* This is a header file to helper stuff.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#ifndef TEXTURE_H
#define TEXTURE_H

#include "Helper.h"
#include <D3DX11tex.h>

class Texture
{
public:
	Texture( void );
	Texture( const Texture& );
	~Texture( void );

	bool Init( ID3D11Device* device, WCHAR* filename );
	void Destroy( void );

	ID3D11ShaderResourceView* GetTexture( void );

private:
	ID3D11ShaderResourceView* texture;
};

#endif // TEXTURE_H
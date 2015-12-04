/*************************************************************************************
* File: Init.h; Date of creation: 10.Feb.2015; Date of Last Modification: 10.Feb.2015
*
* This is a effect source file. This actualy creates effect.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#include "Effect.h"

Effect::Effect( ID3D11Device* device, const std::wstring& filename )
	: fx( 0 )
{
	std::ifstream fin(filename, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();
	
	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size, 0, device, &fx));
}

Effect::~Effect( void )
{
	RELEASE( fx );
}

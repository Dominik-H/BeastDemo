/*************************************************************************************
* File: Init.h; Date of creation: 12.Feb.2015; Date of Last Modification: 12.Feb.2015
*
* This is a header file to helper stuff.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#ifndef SKYBOX_H
#define SKYBOX_H

#include "../Helper.h"
#include "../Camera.h"

class Skybox
{
public:
	Skybox( ID3D11Device* device, const std::wstring& filename, float radius );
	~Skybox( void );

	ID3D11ShaderResourceView* SkyboxSRV( void );

	void Draw( ID3D11DeviceContext* context, Camera* cam );

private:
	Skybox( const Skybox& rhs );
	Skybox& operator=( const Skybox& rhs );

	ID3D11Buffer* VB;
	ID3D11Buffer* IB;

	ID3D11ShaderResourceView* skyboxSRV;

	UINT indexCount;
};

#endif // SKYBOX_H
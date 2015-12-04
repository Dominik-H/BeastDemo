/*************************************************************************************
* File: Init.h; Date of creation: 11.Feb.2015; Date of Last Modification: 11.Feb.2015
*
* This is a header file to helper stuff.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#ifndef OBJECT_3D_H
#define OBJECT_3D_H

#include "Helper.h"
#include "Structs.h"
#include "Camera.h"

class Object3D
{
public:
	Object3D( void );
	virtual ~Object3D( void );

	virtual void BuildBuffers( ID3D11Device* device ) = 0;
	virtual void Update( float dt ) = 0;
	virtual void Draw( ID3D11Device* device, ID3D11DeviceContext* context, Camera* cam ) = 0;

	void SetMaterial( Material mat );
	void SetWorld( XMFLOAT4X4 world );

	ID3D11Buffer* GetVB( void );
	ID3D11Buffer* GetIB( void );
	XMFLOAT4X4 GetWorld( void );
	Material GetMaterial( void );
	UINT GetIndexCount( void );

protected:
	ID3D11Buffer* VB;
	ID3D11Buffer* IB;

	Material Mat;

	XMFLOAT4X4 World;

	UINT indexCount;
};

#endif // OBJECT_3D_H
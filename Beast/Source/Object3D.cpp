/*************************************************************************************
* File: Init.h; Date of creation: 11.Feb.2015; Date of Last Modification: 11.Feb.2015
*
* This is a header file to helper stuff.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#include "Object3D.h"

Object3D::Object3D( void )
	: VB(), IB(), Mat()
{
	indexCount = 0;
	XMStoreFloat4x4( &World, XMMatrixIdentity() );
}


Object3D::~Object3D( void )
{
	RELEASE( VB );
	RELEASE( IB );
}

void Object3D::SetMaterial( Material mat )
{
	Mat = mat;
}
	
void Object3D::SetWorld( XMFLOAT4X4 world )
{
	World = world;
}

ID3D11Buffer* Object3D::GetVB( void )
{
	return VB;
}

ID3D11Buffer* Object3D::GetIB( void )
{
	return IB;
}

XMFLOAT4X4 Object3D::GetWorld( void )
{
	return World;
}

Material Object3D::GetMaterial( void )
{
	return Mat;
}
	
UINT Object3D::GetIndexCount( void )
{
	return indexCount;
}
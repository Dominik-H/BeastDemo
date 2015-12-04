/*************************************************************************************
* File: Init.h; Date of creation: 11.Feb.2015; Date of Last Modification: 11.Feb.2015
*
* This is a header file to helper stuff.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#ifndef BOX_H
#define BOX_H

#include "../Object3D.h"
#include "../Effects.h"
#include "../Camera.h"
#include "../Texture.h"

class Box :	public Object3D
{
public:
	Box( );
	~Box( void );

	virtual void BuildBuffers( ID3D11Device* device );
	virtual void Update( float dt );
	virtual void Draw( ID3D11Device* device, ID3D11DeviceContext* context, Camera* cam );

	void SetTexture( Texture* tex );
	void SetNormal( Texture* norm );
	void SetTexTransform( XMFLOAT4X4 trans );

private:
	Texture* texture;
	Texture* normal;

	XMFLOAT4X4 TexTransform;
};

#endif // BOX_H
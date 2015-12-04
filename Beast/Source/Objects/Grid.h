/*************************************************************************************
* File: Init.h; Date of creation: 12.Feb.2015; Date of Last Modification: 12.Feb.2015
*
* This is a header file to helper stuff.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#ifndef GRID_H
#define GRID_H

#include "../Object3D.h"
#include "../Effects.h"
#include "../Camera.h"
#include "../Texture.h"
#include "../Vertex.h"

class Grid :	public Object3D
{
public:
	Grid(void);
	~Grid(void);

	virtual void BuildBuffers( ID3D11Device* device );
	virtual void Update( float dt );
	virtual void Draw( ID3D11Device* device, ID3D11DeviceContext* context, Camera* cam );

	void SetTexture( Texture* tex );
	void SetNormal( Texture* norm );
	void SetTexTransform( XMFLOAT4X4 trans );

	std::vector<Vertex::Basic> Vertices;
	std::vector<UINT> Indices;

private:
	Texture* texture;
	Texture* normal;

	XMFLOAT4X4 TexTransform;
};

#endif // GRID_H
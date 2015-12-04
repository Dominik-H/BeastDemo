/*************************************************************************************
* File: Init.h; Date of creation: 10.Feb.2015; Date of Last Modification: 10.Feb.2015
*
* This is a header file that stores definitions for Render Input Structures.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#ifndef VERTEX_H
#define VERTEX_H

#include "Helper.h"

namespace Vertex
{
	struct Basic
	{
		Basic() : Pos( 0.0f, 0.0f, 0.0f ), Normal( 0.0f, 0.0f, 0.0f ), Tex( 0.0f, 0.0f ) {}
		Basic( const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT2& t ) 
			: Pos( p ), Normal( n ), Tex( t ) {}
		Basic( float px, float py, float pz, float nx, float ny, float nz, float tx, float ty )
			: Pos ( px, py, pz ), Normal( nx, ny, nz ), Tex( tx, ty ) {}

		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
	};

	struct Terrain
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
		XMFLOAT2 BoundsY;
	};

	struct PosNormalTexTanSkinned
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
		XMFLOAT4 TangentU;
		XMFLOAT3 Weights;
		BYTE BoneIndices[4];
	};

	inline bool operator==(const PosNormalTexTanSkinned& lhs, const PosNormalTexTanSkinned& rhs)
	{
		const XMFLOAT2 vector2Epsilon = XMFLOAT2(0.00001f, 0.00001f);
		const XMFLOAT3 vector3Epsilon = XMFLOAT3(0.00001f, 0.00001f, 0.00001f);
		const XMFLOAT4 vector4Epsilon = XMFLOAT4(0.00001f, 0.00001f, 0.00001f, 0.00001f);

		if (!XMVector3NearEqual(XMLoadFloat3(&lhs.Pos), XMLoadFloat3(&rhs.Pos), XMLoadFloat3(&vector3Epsilon)))
			return false;

		if (!XMVector3NearEqual(XMLoadFloat3(&lhs.Normal), XMLoadFloat3(&rhs.Normal), XMLoadFloat3(&vector3Epsilon)))
			return false;
		
		if (!XMVector2NearEqual(XMLoadFloat2(&lhs.Tex), XMLoadFloat2(&rhs.Tex), XMLoadFloat2(&vector2Epsilon)))
			return false;
		
		if (!XMVector4NearEqual(XMLoadFloat4(&lhs.TangentU), XMLoadFloat4(&rhs.TangentU), XMLoadFloat4(&vector4Epsilon)))
			return false;

		if (!XMVector3NearEqual(XMLoadFloat3(&lhs.Weights), XMLoadFloat3(&rhs.Weights), XMLoadFloat3(&vector3Epsilon)))
			return false;

		if (lhs.BoneIndices[0] != rhs.BoneIndices[0] || lhs.BoneIndices[3] != rhs.BoneIndices[3] || lhs.BoneIndices[2] != rhs.BoneIndices[2] || lhs.BoneIndices[1] != rhs.BoneIndices[1])
			return false;

		return true;
	}

	struct PosNormalTexTan
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
		XMFLOAT4 TangentU;
	};

	inline bool operator==(const PosNormalTexTan& lhs, const PosNormalTexTan& rhs)
	{
		const XMFLOAT2 vector2Epsilon = XMFLOAT2(0.00001f, 0.00001f);
		const XMFLOAT3 vector3Epsilon = XMFLOAT3(0.00001f, 0.00001f, 0.00001f);
		const XMFLOAT4 vector4Epsilon = XMFLOAT4(0.00001f, 0.00001f, 0.00001f, 0.00001f);

		if (!XMVector3NearEqual(XMLoadFloat3(&lhs.Pos), XMLoadFloat3(&rhs.Pos), XMLoadFloat3(&vector3Epsilon)))
			return false;

		if (!XMVector3NearEqual(XMLoadFloat3(&lhs.Normal), XMLoadFloat3(&rhs.Normal), XMLoadFloat3(&vector3Epsilon)))
			return false;

		if (!XMVector2NearEqual(XMLoadFloat2(&lhs.Tex), XMLoadFloat2(&rhs.Tex), XMLoadFloat2(&vector2Epsilon)))
			return false;

		if (!XMVector4NearEqual(XMLoadFloat4(&lhs.TangentU), XMLoadFloat4(&rhs.TangentU), XMLoadFloat4(&vector4Epsilon)))
			return false;

		return true;
	}
}

class InputLayouts
{
public:
	static void Init( ID3D11Device* device );
	static void Destroy( void );

	static ID3D11InputLayout* Basic;
	static ID3D11InputLayout* Pos;
	static ID3D11InputLayout* Terrain;
	static ID3D11InputLayout* PosNormalTexTanSkinned;
	static ID3D11InputLayout* PosNormalTexTan;
};

#endif // VERTEX_H
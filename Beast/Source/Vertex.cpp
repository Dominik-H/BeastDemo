/*************************************************************************************
* File: Init.h; Date of creation: 10.Feb.2015; Date of Last Modification: 10.Feb.2015
*
* This is a file that initializes Inputs for Render stages.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#include "Vertex.h"
#include "Effects.h"

namespace ILDesc
{
	const D3D11_INPUT_ELEMENT_DESC Basic[3] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	12,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	24,	D3D11_INPUT_PER_VERTEX_DATA,	0 }
	};

	const D3D11_INPUT_ELEMENT_DESC Pos[1] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	const D3D11_INPUT_ELEMENT_DESC Terrain[3] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	const D3D11_INPUT_ELEMENT_DESC PosNormalTexTanSkinned[6] =
	{
		{"POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",       0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"WEIGHTS",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEINDICES",  0, DXGI_FORMAT_R8G8B8A8_UINT,   0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	
	static const D3D11_INPUT_ELEMENT_DESC PosNormalTexTan[4] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
}

ID3D11InputLayout* InputLayouts::Basic									= 0;
ID3D11InputLayout* InputLayouts::Pos										= 0;
ID3D11InputLayout* InputLayouts::Terrain								= 0;
ID3D11InputLayout* InputLayouts::PosNormalTexTanSkinned = 0;
ID3D11InputLayout* InputLayouts::PosNormalTexTan				 = 0;

void InputLayouts::Init( ID3D11Device* device )
{
	D3DX11_PASS_DESC pDesc;

	//--------------------- 
	// Basic
	//---------------------

	Effects::BasicFX->Light1Tech->GetPassByIndex( 0 )->GetDesc( &pDesc );
	HR( device->CreateInputLayout( ILDesc::Basic, 3, pDesc.pIAInputSignature, pDesc.IAInputSignatureSize, &Basic ) );
	
	//---------------------
	// Pos
	//---------------------

	Effects::SkyFX->SkyTech->GetPassByIndex( 0 )->GetDesc( &pDesc );
	HR( device->CreateInputLayout( ILDesc::Pos, 1, pDesc.pIAInputSignature, 
		pDesc.IAInputSignatureSize, &Pos ) );
	
	//---------------------
	// Terrain
	//---------------------

	Effects::TerrainFX->Light1Tech->GetPassByIndex(0)->GetDesc(&pDesc);
	HR(device->CreateInputLayout(ILDesc::Terrain, 3, pDesc.pIAInputSignature, 
		pDesc.IAInputSignatureSize, &Terrain));

	//---------------------
	// NormalMapSkinned
	//---------------------

	Effects::NormalMapFX->Light1SkinnedTech->GetPassByIndex(0)->GetDesc(&pDesc);
	HR(device->CreateInputLayout(ILDesc::PosNormalTexTanSkinned, 6, pDesc.pIAInputSignature, 
		pDesc.IAInputSignatureSize, &PosNormalTexTanSkinned));

	//---------------------
	// NormalMap
	//---------------------

	Effects::NormalMapFX->Light1Tech->GetPassByIndex(0)->GetDesc(&pDesc);
	HR(device->CreateInputLayout(ILDesc::PosNormalTexTan, 4, pDesc.pIAInputSignature, 
		pDesc.IAInputSignatureSize, &PosNormalTexTan));
}

void InputLayouts::Destroy()
{
	RELEASE( Basic );
	RELEASE( Pos );
	RELEASE( Terrain );
	RELEASE( PosNormalTexTanSkinned );
	RELEASE( PosNormalTexTan );
}
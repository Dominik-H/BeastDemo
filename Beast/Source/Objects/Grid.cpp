/*************************************************************************************
* File: Init.h; Date of creation: 12.Feb.2015; Date of Last Modification: 12.Feb.2015
*
* This is a header file to helper stuff.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#include "Grid.h"
#include "ShapeGenerator.h"
#include "../Vertex.h"

Grid::Grid()
	: Object3D(), texture( 0 ), normal( 0 )
{
	XMStoreFloat4x4( &TexTransform, XMMatrixScaling( 750.0f, 750.0f, 750.0f ) );
}

void Grid::SetTexture( Texture* tex )
{
	texture = tex;
}
	
void Grid::SetNormal( Texture* norm )
{
	normal = norm;
}

void Grid::SetTexTransform( XMFLOAT4X4 trans )
{
	TexTransform = trans;
}

Grid::~Grid( void )
{
}

void Grid::BuildBuffers( ID3D11Device* device )
{
	ShapeGenerator::MeshData grid;

	ShapeGenerator shapeGen;
	shapeGen.CreateGrid( 5000.0f, 5000.0f, 1000, 1000, grid );

	std::vector<Vertex::Basic> vertices( grid.Vertices.size() );
	for( UINT i = 0; i < grid.Vertices.size(); ++i )
	{
		vertices[i].Pos			= grid.Vertices[i].Position;
		vertices[i].Normal	= grid.Vertices[i].Normal;
		vertices[i].Tex			= grid.Vertices[i].TexC;
	}
	
	Vertices = vertices;
	Indices = grid.Indices;

	D3D11_BUFFER_DESC vbd;
  vbd.Usage = D3D11_USAGE_IMMUTABLE;
  vbd.ByteWidth = sizeof(Vertex::Basic) * grid.Vertices.size();
  vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vbd.CPUAccessFlags = 0;
  vbd.MiscFlags = 0;
  D3D11_SUBRESOURCE_DATA vinitData;
  vinitData.pSysMem = &vertices[0];
  HR(device->CreateBuffer(&vbd, &vinitData, &VB));

	indexCount = grid.Indices.size();

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * grid.Indices.size();
  ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
  ibd.CPUAccessFlags = 0;
  ibd.MiscFlags = 0;
  D3D11_SUBRESOURCE_DATA iinitData;
  iinitData.pSysMem = &grid.Indices[0];
  HR(device->CreateBuffer(&ibd, &iinitData, &IB));
}

void Grid::Update( float dt )
{

}

void Grid::Draw( ID3D11Device* device, ID3D11DeviceContext* context, Camera* cam )
{
	context->IASetInputLayout( InputLayouts::Basic );
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof( Vertex::Basic );
	UINT offset = 0;

	XMMATRIX view     = cam->View();
	XMMATRIX proj     = cam->Proj();
	XMMATRIX viewProj = cam->ViewProj();

	ID3DX11EffectTechnique* activeTech = Effects::BasicFX->Light3TexTech;

	D3DX11_TECHNIQUE_DESC techDesc;
  activeTech->GetDesc( &techDesc );
	for( UINT p = 0; p < techDesc.Passes; ++p )
  {
		context->IASetVertexBuffers( 0, 1, &VB, &stride, &offset );
		context->IASetIndexBuffer( IB, DXGI_FORMAT_R32_UINT, 0 );

		XMMATRIX world = XMLoadFloat4x4( &World );
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose( world );
		XMMATRIX worldViewProj = world*view*proj;

		Effects::BasicFX->SetWorld( world );
		Effects::BasicFX->SetWorldInvTranspose( worldInvTranspose );
		Effects::BasicFX->SetWorldViewProj( worldViewProj );
		Effects::BasicFX->SetDiffuseMap( texture->GetTexture() );
		Effects::BasicFX->SetTexTransform( XMLoadFloat4x4( &TexTransform ) );
		Effects::BasicFX->SetMaterial( Mat );

		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(indexCount, 0, 0);
	}
}
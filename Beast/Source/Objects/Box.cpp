/*************************************************************************************
* File: Init.h; Date of creation: 11.Feb.2015; Date of Last Modification: 11.Feb.2015
*
* This is a header file to helper stuff.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#include "Box.h"
#include "ShapeGenerator.h"
#include "../Vertex.h"

Box::Box()
	: Object3D(), texture( 0 ), normal( 0 )
{
	XMStoreFloat4x4( &TexTransform, XMMatrixIdentity() );
}

void Box::SetTexture( Texture* tex )
{
	texture = tex;
}
	
void Box::SetNormal( Texture* norm )
{
	normal = norm;
}

void Box::SetTexTransform( XMFLOAT4X4 trans )
{
	TexTransform = trans;
}

Box::~Box( void )
{
}

void Box::BuildBuffers( ID3D11Device* device )
{
	ShapeGenerator::MeshData box;

	ShapeGenerator shapeGen;
	shapeGen.CreateBox( 1.0f, 1.0f, 1.0f, box );

	std::vector<Vertex::Basic> vertices( box.Vertices.size() );
	for( UINT i = 0; i < box.Vertices.size(); ++i )
	{
		vertices[i].Pos			= box.Vertices[i].Position;
		vertices[i].Normal	= box.Vertices[i].Normal;
		vertices[i].Tex			= box.Vertices[i].TexC;
	}

	D3D11_BUFFER_DESC vbd;
  vbd.Usage = D3D11_USAGE_IMMUTABLE;
  vbd.ByteWidth = sizeof(Vertex::Basic) * box.Vertices.size();
  vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vbd.CPUAccessFlags = 0;
  vbd.MiscFlags = 0;
  D3D11_SUBRESOURCE_DATA vinitData;
  vinitData.pSysMem = &vertices[0];
  HR(device->CreateBuffer(&vbd, &vinitData, &VB));

	indexCount = box.Indices.size();

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * box.Indices.size();
  ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
  ibd.CPUAccessFlags = 0;
  ibd.MiscFlags = 0;
  D3D11_SUBRESOURCE_DATA iinitData;
  iinitData.pSysMem = &box.Indices[0];
  HR(device->CreateBuffer(&ibd, &iinitData, &IB));
}

void Box::Update( float dt )
{

}

void Box::Draw( ID3D11Device* device, ID3D11DeviceContext* context, Camera* cam )
{
	context->IASetInputLayout( InputLayouts::Basic );
	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

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
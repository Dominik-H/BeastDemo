/*************************************************************************************
* File: Init.h; Date of creation: 12.Feb.2015; Date of Last Modification: 12.Feb.2015
*
* This is a header file to helper stuff.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#include "Skybox.h"
#include "ShapeGenerator.h"
#include "../Vertex.h"
#include "../Effects.h"

Skybox::Skybox( ID3D11Device* device, const std::wstring& filename, float radius )
{
	HR( D3DX11CreateShaderResourceViewFromFile ( device, filename.c_str(), 0, 0, &skyboxSRV, 0 ) );

	ShapeGenerator::MeshData sphere;
	ShapeGenerator geoGen;
	geoGen.CreateSphere( radius, 30, 30, sphere );

	std::vector<XMFLOAT3> vertices( sphere.Vertices.size() );

	for( size_t i = 0; i < sphere.Vertices.size(); ++i )
	{
		vertices[i] = sphere.Vertices[i].Position;
	}

  D3D11_BUFFER_DESC vbd;
  vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(XMFLOAT3) * vertices.size();
  vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vbd.CPUAccessFlags = 0;
  vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

  D3D11_SUBRESOURCE_DATA vinitData;
  vinitData.pSysMem = &vertices[0];

  HR(device->CreateBuffer( &vbd, &vinitData, &VB ) );
	

	indexCount = sphere.Indices.size();

	D3D11_BUFFER_DESC ibd;
  ibd.Usage = D3D11_USAGE_IMMUTABLE;
  ibd.ByteWidth = sizeof(USHORT) * indexCount;
  ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
  ibd.CPUAccessFlags = 0;
	ibd.StructureByteStride = 0;
  ibd.MiscFlags = 0;

	std::vector<USHORT> indices;
	indices.assign(sphere.Indices.begin(), sphere.Indices.end());

  D3D11_SUBRESOURCE_DATA iinitData;
  iinitData.pSysMem = &indices[0];

  HR( device->CreateBuffer( &ibd, &iinitData, &IB ) );
}

Skybox::~Skybox()
{
	RELEASE( VB );
	RELEASE( IB );
	RELEASE( skyboxSRV );
}

ID3D11ShaderResourceView* Skybox::SkyboxSRV()
{
	return skyboxSRV;
}

void Skybox::Draw( ID3D11DeviceContext* context, Camera* cam )
{
	// center Sky about eye in world space
	XMFLOAT3 eyePos = cam->GetPosition();
	XMMATRIX T = XMMatrixTranslation(eyePos.x, eyePos.y, eyePos.z);

	XMMATRIX WVP = XMMatrixMultiply(T, cam->ViewProj());

	Effects::SkyFX->SetWorldViewProj(WVP);
	Effects::SkyFX->SetCubeMap(skyboxSRV);

	UINT stride = sizeof(XMFLOAT3);
  UINT offset = 0;
  context->IASetVertexBuffers(0, 1, &VB, &stride, &offset);
	context->IASetIndexBuffer(IB, DXGI_FORMAT_R16_UINT, 0);
	context->IASetInputLayout(InputLayouts::Pos);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	D3DX11_TECHNIQUE_DESC techDesc;
  Effects::SkyFX->SkyTech->GetDesc( &techDesc );

  for(UINT p = 0; p < techDesc.Passes; ++p)
  {
		ID3DX11EffectPass* pass = Effects::SkyFX->SkyTech->GetPassByIndex(p);

		pass->Apply(0, context);

		context->DrawIndexed(indexCount, 0, 0);
	}
}
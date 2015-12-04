#include "Level0.h"


Level0::Level0()
{
}


Level0::~Level0()
{
	//tex.Destroy();
	//norm.Destroy();
	DEL(sky);
}

bool Level0::LoadLevel(ID3D11Device *device, ID3D11DeviceContext *context)
{
	sky = new Skybox(device, L"Textures/Skybox.dds", 5000.0f);
	grid.BuildBuffers(device);

#if defined( DEBUG ) || defined( _DEBUG )
	Collider.CreateStaticD(device, grid.Vertices);
#else
	Collider.CreateStatic(grid.Vertices);
#endif

	gridMat.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	gridMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	gridMat.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	tex.Init(device, L"Textures/floor.dds");
	norm.Init(device, L"Textures/floor_nmap.dds");

	grid.SetTexture(&tex);
	grid.SetNormal(&norm);
	grid.SetMaterial(gridMat);

	/*dirLights[0].Ambient = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	dirLights[0].Diffuse = XMFLOAT4(0.6f, 0.6f, 0.5f, 1.0f);
	dirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.4f, 1.0f);
	dirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	dirLights[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	dirLights[1].Diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	dirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	dirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	dirLights[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	dirLights[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	dirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	dirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);*/

	dirLights[0].Ambient = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	dirLights[0].Diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	dirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.4f, 1.0f);
	dirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	dirLights[1].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	dirLights[1].Diffuse = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	dirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	dirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	dirLights[2].Ambient = XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f);
	dirLights[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	dirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	dirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);

	return true;
}

void Level0::Update(float dt)
{

}

void Level0::Draw(ID3D11Device *device, ID3D11DeviceContext *context)
{
	sky->Draw(context, cam);
	grid.Draw(device, context, cam);
	/*
	for (UINT i = 0; i < objects.size; ++i)
	{
		objects[i]->Draw(device, context, cam, dirLights, sky);
	}*/
}

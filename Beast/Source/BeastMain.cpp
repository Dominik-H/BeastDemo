#if 0
/*************************************************************************************
* File: Init.h; Date of creation: 09.Feb.2015; Date of Last Modification: 12.Feb.2015
*
* This is the actual game that connects everything so that GameCore class can run it. 
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#include "BeastMain.h"
#include "Vertex.h"

Beast::Beast( void )
	: sky( 0 ), charModel( 0 )
{	
	dirLights[0].Ambient  = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	dirLights[0].Diffuse  = XMFLOAT4(0.6f, 0.6f, 0.5f, 1.0f);
	dirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.4f, 1.0f);
	dirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	dirLights[1].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	dirLights[1].Diffuse  = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	dirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	dirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	dirLights[2].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	dirLights[2].Diffuse  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	dirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	dirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);
	
	Material a;
	a.Ambient  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	a.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	a.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);

	aBox.SetMaterial( a );

	XMFLOAT4X4 box;
	XMStoreFloat4x4( &box, XMMatrixMultiply( XMMatrixTranslation( 0.0f, 10.0f, 0.0f ), XMMatrixScaling(10.0f, 10.0f, 10.0f) ) );
	aBox.SetWorld( box );

	/*a.Ambient  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	a.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	a.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	ground.SetMaterial( a );*/
}

Beast::~Beast( void )
{
	Effects::Destroy();
	InputLayouts::Destroy();
	RenderStates::Destroy();

	boxText.Destroy();
	groundText.Destroy();

	DEL( sky );
	DEL( charModel );

	DIKeyboard->Unacquire();
	DIMouse->Unacquire();
	RELEASE( DirectInput );
}

bool Beast::Init( ID3D11Device* device,	ID3D11DeviceContext* context, IDXGISwapChain* sc, HINSTANCE hInst, HWND hwnd )
{
	if( !InitDirectInput( hInst, hwnd ) )
		return false;

	Effects::Init( device );
	InputLayouts::Init( device );
	RenderStates::Init( device );

	texMgr.Init( device );
	boxText.Init( device, L"Textures/stone.dds" );
	//groundText.Init( device, L"Textures/floor.dds" );

	sky = new Skybox( device, L"Textures/Skybox.dds", 5000.0f );

	aBox.SetTexture( &boxText );
	aBox.BuildBuffers( device );

	//ground.SetTexture( &groundText );
	//ground.BuildBuffers( device );

	Terrain::InitInfo tii;
	tii.HeightMapFilename = L"Textures/heightmap.raw";
	tii.LayerMapFilename0 = L"Textures/grass.dds";
	tii.LayerMapFilename1 = L"Textures/darkdirt.dds";
	tii.LayerMapFilename2 = L"Textures/stone.dds";
	tii.LayerMapFilename3 = L"Textures/lightdirt.dds";
	tii.LayerMapFilename4 = L"Textures/snow.dds";
	tii.BlendMapFilename = L"Textures/blend.dds";
	tii.HeightScale = 50.0f;
	tii.HeightmapWidth = 2049;
	tii.HeightmapHeight = 2049;
	tii.CellSpacing = 0.5f;

	terrain.Init( device, context, tii);

	std::string a = "Models\\CODMapShipment\\Files\\CODMapShipment.obj";
	Model16::InitInfo inf;
	inf.NumLights = 3;
	inf.Mgr = &texMgr;
	inf.pDevice = device;
	inf.pDeviceContext = context;
	inf.UseDefaultMaterial = true;

	map.SetInitInfo(inf);
	map.LoadModel(a);

	charModel = new SkinnedModel( device, texMgr, "Models\\soldier.m3d", L"Textures\\" );

	modelInst1.Model = charModel;
	modelInst1.TimePos = 0.0f;
	modelInst1.ClipName = "Take1";
	modelInst1.FinalTransforms.resize( charModel->SkinnedData.BoneCount() );
	modelInst1.position.x = 2.0f;
	modelInst1.position.y = terrain.GetHeight(2.0f, -7.0f);
	modelInst1.position.z = -7.0f;
	modelInst1.fwd = cam.GetLook();
	modelInst1.right = cam.GetRight();
	modelInst1.up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	modelInst1.scale.x = 0.05f;
	modelInst1.scale.y = 0.05f;
	modelInst1.scale.z = -0.05f;
	modelInst1.isWalking = true;
	modelInst1.Update( 0.0f );
	modelInst1.isWalking = false;
	modelInst1.Update( 0.0f );

	cam.SetPosition( modelInst1.position.x, modelInst1.position.y + 8, -15.0f );

	return true;
}

void Beast::OnResize( int width, int height, float aspect )
{
	cam.SetLens( 0.25f*MathHelper::Pi, aspect, 1.0f, 1000.0f );
}

void Beast::Update( float dt )
{
	// object.Update( dt );
	modelInst1.Update( dt );
	modelInst1.isWalking = false;

	cam.ClampCamera( modelInst1.position.y );
	cam.UpdateViewMatrix();
}

void Beast::Draw( ID3D11Device* device, ID3D11DeviceContext* context )
{
	//--------------------------------------------------
	// Set Per-Frame Constants
	//--------------------------------------------------
	Effects::BasicFX->SetDirLights( dirLights );
	Effects::BasicFX->SetEyePosW( cam.GetPosition() );

	//context->RSSetState( RenderStates::NoCullRS );
	aBox.Draw( device, context, &cam );
	//context->RSSetState( RenderStates::WireframeRS );

	//ground.Draw( device, context, &cam );
	terrain.Draw( context, &cam, dirLights );
	context->RSSetState( RenderStates::NoCullRS );
	map.Render(XMMatrixIdentity(), &cam);
	context->RSSetState( 0 );

	DrawModel( device, context );

	sky->Draw( context, &cam );
}

bool Beast::InitDirectInput( HINSTANCE hInst, HWND hwnd )
{
	DirectInput8Create( hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, ( void ** )&DirectInput, NULL );

	HR( DirectInput->CreateDevice( GUID_SysKeyboard, &DIKeyboard, NULL ) );
	HR( DirectInput->CreateDevice( GUID_SysMouse, &DIMouse, NULL ) );
	
	HR( DIKeyboard->SetDataFormat( &c_dfDIKeyboard ) );
	HR( DIKeyboard->SetCooperativeLevel( hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) );

	HR( DIMouse->SetDataFormat( &c_dfDIMouse ) );
	HR( DIMouse->SetCooperativeLevel( hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND ) );

	return true;
}

void Beast::DetectInput( float dt, HWND hwnd )
{
	DIMOUSESTATE currMouseState;
	BYTE keyboardState[256];

	DIKeyboard->Acquire();
	DIMouse->Acquire();

	DIMouse->GetDeviceState( sizeof( DIMOUSESTATE ), &currMouseState );
	DIKeyboard->GetDeviceState( sizeof( keyboardState ), (LPVOID)&keyboardState );

	if(keyboardState[DIK_ESCAPE] & 0x80)
		PostMessage(hwnd, WM_DESTROY, 0, 0);
	if(keyboardState[DIK_W] & 0x80)
	{
		modelInst1.isWalking = true;
		modelInst1.Walk( 10*dt, &cam, &terrain );
		cam.Walk( 10 * dt );
	}
	if(keyboardState[DIK_S] & 0x80)
	{
		modelInst1.isWalking = true;
		modelInst1.Walk(-10 * dt, &cam, &terrain);
		cam.Walk( -10 * dt );
	}
	if(keyboardState[DIK_A] & 0x80)
	{
		modelInst1.isWalking = true;
		modelInst1.Strafe(-10 * dt, &cam, &terrain);
		cam.Strafe( -10 * dt );
	}
	if(keyboardState[DIK_D] & 0x80)
	{
		modelInst1.isWalking = true;
		modelInst1.Strafe(10 * dt, &cam, &terrain);
		cam.Strafe( 10 * dt );
	}

	if( currMouseState.lX != mouseLastState.lX )
	{
		cam.SetPosition(modelInst1.position);
		cam.RotateY( XMConvertToRadians( 0.25f * static_cast<float>(currMouseState.lX ) ) );
		XMFLOAT3 camPos;
		camPos.x = modelInst1.position.x - (8 * cam.GetLook().x );
		camPos.y = modelInst1.position.y - (8 * cam.GetLook().y );
		camPos.z = modelInst1.position.z - (8 * cam.GetLook().z );
		cam.SetPosition(camPos);

		modelInst1.RotateY( XMConvertToRadians( 0.25f * static_cast<float>(currMouseState.lX ) ) );
	}
	if( currMouseState.lY != mouseLastState.lY )
	{
		cam.SetPosition(modelInst1.position);
		cam.Pitch( XMConvertToRadians( 0.12f * static_cast<float>(currMouseState.lY) ) );
		XMFLOAT3 camPos;
		camPos.x = modelInst1.position.x - (8 * cam.GetLook().x );
		camPos.y = modelInst1.position.y - (8 * cam.GetLook().y );
		camPos.z = modelInst1.position.z - (8 * cam.GetLook().z );
		cam.SetPosition(camPos);
	}

	mouseLastState = currMouseState;

	return;
}

void Beast::DrawModel( ID3D11Device* device, ID3D11DeviceContext* context )
{
	// DOCASNE!!!!!!! Model
	XMMATRIX view     = cam.View();
	XMMATRIX proj     = cam.Proj();
	XMMATRIX viewProj = cam.ViewProj();

	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
	XMMATRIX toTexSpace(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	Effects::NormalMapFX->SetDirLights(dirLights);
	Effects::NormalMapFX->SetEyePosW(cam.GetPosition());
	Effects::NormalMapFX->SetCubeMap(sky->SkyboxSRV());

	context->IASetInputLayout( InputLayouts::PosNormalTexTanSkinned );
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DX11_TECHNIQUE_DESC techDesc;
	ID3DX11EffectTechnique* tech = Effects::NormalMapFX->Light3TexSkinnedTech;

	tech->GetDesc( &techDesc );
	for( UINT p = 0; p < techDesc.Passes; ++p )
	{
		XMMATRIX world = XMLoadFloat4x4( &modelInst1.World );
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose( world );
		XMMATRIX worldViewProj = world*view*proj;

		Effects::NormalMapFX->SetWorld(world);
		Effects::NormalMapFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::NormalMapFX->SetWorldViewProj(worldViewProj);
		Effects::NormalMapFX->SetWorldViewProjTex(worldViewProj*toTexSpace);
		//Effects::NormalMapFX->SetShadowTransform(world*shadowTransform);
		Effects::NormalMapFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
		Effects::NormalMapFX->SetBoneTransforms(
			&modelInst1.FinalTransforms[0], 
			modelInst1.FinalTransforms.size());

		for(UINT subset = 0; subset < modelInst1.Model->SubsetCount; ++subset)
		{
			Effects::NormalMapFX->SetMaterial(modelInst1.Model->Mat[subset]);
			Effects::NormalMapFX->SetDiffuseMap(modelInst1.Model->DiffuseMapSRV[subset]);
			Effects::NormalMapFX->SetNormalMap(modelInst1.Model->NormalMapSRV[subset]);

			tech->GetPassByIndex(p)->Apply(0, context);
			modelInst1.Model->ModelMesh.Draw(context, subset);
		}
	}
}

#endif
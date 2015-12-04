/*************************************************************************************
* File: Init.h; Date of creation: 10.Feb.2015; Date of Last Modification: 10.Feb.2015
*
* Init calls constructor to create all the effects and Destroy deletes all the effects.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#include "Effects.h"

BasicEffect*			Effects::BasicFX			= 0;
SkyEffect*				Effects::SkyFX				= 0;
TerrainEffect*		Effects::TerrainFX		= 0;
NormalMapEffect*	Effects::NormalMapFX	= 0;
SpriteEffect*		Effects::SpriteFX		= 0;

void Effects::Init( ID3D11Device* device )
{
	BasicFX = new BasicEffect( device, L"Shaders/Basic.fxo" );
	SkyFX = new SkyEffect( device, L"Shaders/Sky.fxo" );
	TerrainFX = new TerrainEffect( device, L"Shaders/Terrain.fxo" );
	NormalMapFX = new NormalMapEffect(device, L"Shaders/NormalMap.fxo");
	SpriteFX = new SpriteEffect(device, L"Shaders/Sprite.fxo");
}


void Effects::Destroy( void )
{
	DEL( BasicFX );
	DEL( SkyFX );
	DEL( TerrainFX );
	DEL( NormalMapFX );
	DEL( SpriteFX );
}

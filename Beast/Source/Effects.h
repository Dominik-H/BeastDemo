/*************************************************************************************
* File: Init.h; Date of creation: 10.Feb.2015; Date of Last Modification: 10.Feb.2015
*
* This is a class that stores all the effects at one place.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#ifndef EFFECTS_H
#define EFFECTS_H

#include "FX/BasicEffect.h"
#include "FX/SkyEffect.h"
#include "FX/TerrainEffect.h"
#include "FX/NormalMapEffect.h"
#include "FX/SpriteEffect.h"

class Effects
{
public:
	static void Destroy( void );
	static void Init( ID3D11Device* device );

	//--------------------------------------- 
	// Actual Effects
	//--------------------------------------
	static BasicEffect* BasicFX;
	static SkyEffect* SkyFX;
	static TerrainEffect* TerrainFX;
	static NormalMapEffect* NormalMapFX;
	static SpriteEffect* SpriteFX;
};

#endif EFFECTS_H
/*************************************************************************************
* File: Init.h; Date of creation: 13.Feb.2015; Date of Last Modification: 13.Feb.2015
*
* This is a header file to helper stuff.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#include "TerrainEffect.h"

TerrainEffect::TerrainEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	Light1Tech    = fx->GetTechniqueByName("Light1");
	Light2Tech    = fx->GetTechniqueByName("Light2");
	Light3Tech    = fx->GetTechniqueByName("Light3");
	Light1FogTech = fx->GetTechniqueByName("Light1Fog");
	Light2FogTech = fx->GetTechniqueByName("Light2Fog");
	Light3FogTech = fx->GetTechniqueByName("Light3Fog");

	ViewProj           = fx->GetVariableByName("gViewProj")->AsMatrix();
	EyePosW            = fx->GetVariableByName("gEyePosW")->AsVector();
	FogColor           = fx->GetVariableByName("gFogColor")->AsVector();
	FogStart           = fx->GetVariableByName("gFogStart")->AsScalar();
	FogRange           = fx->GetVariableByName("gFogRange")->AsScalar();
	DirLights          = fx->GetVariableByName("gDirLights");
	Mat                = fx->GetVariableByName("gMaterial");

	MinDist            = fx->GetVariableByName("gMinDist")->AsScalar();
	MaxDist            = fx->GetVariableByName("gMaxDist")->AsScalar();
	MinTess            = fx->GetVariableByName("gMinTess")->AsScalar();
	MaxTess            = fx->GetVariableByName("gMaxTess")->AsScalar();
	//ShadowTransform   = fx->GetVariableByName("gShadowTransform")->AsMatrix();
	TexelCellSpaceU    = fx->GetVariableByName("gTexelCellSpaceU")->AsScalar();
	TexelCellSpaceV    = fx->GetVariableByName("gTexelCellSpaceV")->AsScalar();
	WorldCellSpace     = fx->GetVariableByName("gWorldCellSpace")->AsScalar();
	WorldFrustumPlanes = fx->GetVariableByName("gWorldFrustumPlanes")->AsVector();

	LayerMapArray      = fx->GetVariableByName("gLayerMapArray")->AsShaderResource();
	BlendMap           = fx->GetVariableByName("gBlendMap")->AsShaderResource();
	HeightMap          = fx->GetVariableByName("gHeightMap")->AsShaderResource();
	//ShadowMap         = fx->GetVariableByName("gShadowMap")->AsShaderResource();
}

TerrainEffect::~TerrainEffect()
{
}

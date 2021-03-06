#pragma once

#include "Effect.h"

class NormalMapEffect : public Effect
{
public:
	NormalMapEffect(ID3D11Device* device, const std::wstring& filename);
	~NormalMapEffect();

	void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProjTex(CXMMATRIX M)               { WorldViewProjTex->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M)                          { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M)              { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetBoneTransforms(const XMFLOAT4X4* M, int cnt){ BoneTransforms->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt); }
	//void SetShadowTransform(CXMMATRIX M)                { ShadowTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(CXMMATRIX M)                   { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetFogColor(const FXMVECTOR v)                 { FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	void SetFogStart(float f)                           { FogStart->SetFloat(f); }
	void SetFogRange(float f)                           { FogRange->SetFloat(f); }
	void SetDirLights(const DirectionalLight* lights)   { DirLights->SetRawValue(lights, 0, 3*sizeof(DirectionalLight)); }
	void SetMaterial(const Material& mat)               { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* tex)   { DiffuseMap->SetResource(tex); }
	void SetCubeMap(ID3D11ShaderResourceView* tex)      { CubeMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* tex)    { NormalMap->SetResource(tex); }
	//void SetSsaoMap(ID3D11ShaderResourceView* tex)      { SsaoMap->SetResource(tex); }
	//void SetShadowMap(ID3D11ShaderResourceView* tex)    { ShadowMap->SetResource(tex); }

	ID3DX11EffectTechnique* Light1Tech;
	ID3DX11EffectTechnique* Light2Tech;
	ID3DX11EffectTechnique* Light3Tech;

	ID3DX11EffectTechnique* Light0TexTech;
	ID3DX11EffectTechnique* Light1TexTech;
	ID3DX11EffectTechnique* Light2TexTech;
	ID3DX11EffectTechnique* Light3TexTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipTech;

	ID3DX11EffectTechnique* Light1FogTech;
	ID3DX11EffectTechnique* Light2FogTech;
	ID3DX11EffectTechnique* Light3FogTech;

	ID3DX11EffectTechnique* Light0TexFogTech;
	ID3DX11EffectTechnique* Light1TexFogTech;
	ID3DX11EffectTechnique* Light2TexFogTech;
	ID3DX11EffectTechnique* Light3TexFogTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogTech;

	ID3DX11EffectTechnique* Light1ReflectTech;
	ID3DX11EffectTechnique* Light2ReflectTech;
	ID3DX11EffectTechnique* Light3ReflectTech;

	ID3DX11EffectTechnique* Light0TexReflectTech;
	ID3DX11EffectTechnique* Light1TexReflectTech;
	ID3DX11EffectTechnique* Light2TexReflectTech;
	ID3DX11EffectTechnique* Light3TexReflectTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipReflectTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipReflectTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipReflectTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipReflectTech;

	ID3DX11EffectTechnique* Light1FogReflectTech;
	ID3DX11EffectTechnique* Light2FogReflectTech;
	ID3DX11EffectTechnique* Light3FogReflectTech;

	ID3DX11EffectTechnique* Light0TexFogReflectTech;
	ID3DX11EffectTechnique* Light1TexFogReflectTech;
	ID3DX11EffectTechnique* Light2TexFogReflectTech;
	ID3DX11EffectTechnique* Light3TexFogReflectTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipFogReflectTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogReflectTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipFogReflectTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogReflectTech;

	ID3DX11EffectTechnique* Light1SkinnedTech;
	ID3DX11EffectTechnique* Light2SkinnedTech;
	ID3DX11EffectTechnique* Light3SkinnedTech;

	ID3DX11EffectTechnique* Light0TexSkinnedTech;
	ID3DX11EffectTechnique* Light1TexSkinnedTech;
	ID3DX11EffectTechnique* Light2TexSkinnedTech;
	ID3DX11EffectTechnique* Light3TexSkinnedTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipSkinnedTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipSkinnedTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipSkinnedTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipSkinnedTech;

	ID3DX11EffectTechnique* Light1FogSkinnedTech;
	ID3DX11EffectTechnique* Light2FogSkinnedTech;
	ID3DX11EffectTechnique* Light3FogSkinnedTech;

	ID3DX11EffectTechnique* Light0TexFogSkinnedTech;
	ID3DX11EffectTechnique* Light1TexFogSkinnedTech;
	ID3DX11EffectTechnique* Light2TexFogSkinnedTech;
	ID3DX11EffectTechnique* Light3TexFogSkinnedTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipFogSkinnedTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogSkinnedTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipFogSkinnedTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogSkinnedTech;

	ID3DX11EffectTechnique* Light1ReflectSkinnedTech;
	ID3DX11EffectTechnique* Light2ReflectSkinnedTech;
	ID3DX11EffectTechnique* Light3ReflectSkinnedTech;

	ID3DX11EffectTechnique* Light0TexReflectSkinnedTech;
	ID3DX11EffectTechnique* Light1TexReflectSkinnedTech;
	ID3DX11EffectTechnique* Light2TexReflectSkinnedTech;
	ID3DX11EffectTechnique* Light3TexReflectSkinnedTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipReflectSkinnedTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipReflectSkinnedTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipReflectSkinnedTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipReflectSkinnedTech;

	ID3DX11EffectTechnique* Light1FogReflectSkinnedTech;
	ID3DX11EffectTechnique* Light2FogReflectSkinnedTech;
	ID3DX11EffectTechnique* Light3FogReflectSkinnedTech;

	ID3DX11EffectTechnique* Light0TexFogReflectSkinnedTech;
	ID3DX11EffectTechnique* Light1TexFogReflectSkinnedTech;
	ID3DX11EffectTechnique* Light2TexFogReflectSkinnedTech;
	ID3DX11EffectTechnique* Light3TexFogReflectSkinnedTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipFogReflectSkinnedTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogReflectSkinnedTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipFogReflectSkinnedTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogReflectSkinnedTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProjTex;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* BoneTransforms;
	//ID3DX11EffectMatrixVariable* ShadowTransform;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* FogColor;
	ID3DX11EffectScalarVariable* FogStart;
	ID3DX11EffectScalarVariable* FogRange;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* Mat;

	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* CubeMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
	//ID3DX11EffectShaderResourceVariable* ShadowMap;
	//ID3DX11EffectShaderResourceVariable* SsaoMap;
};
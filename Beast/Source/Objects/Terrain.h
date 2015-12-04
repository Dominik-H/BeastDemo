/*************************************************************************************
* File: Init.h; Date of creation: 13.Feb.2015; Date of Last Modification: 13.Feb.2015
*
* This is a header file to helper stuff.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#ifndef TERRAIN_H
#define TERRAIN_H

#include "../Helper.h"
#include "../Camera.h"
#include "../Structs.h"

class Terrain
{
public:
	struct InitInfo
	{
		std::wstring HeightMapFilename;
		std::wstring LayerMapFilename0;
		std::wstring LayerMapFilename1;
		std::wstring LayerMapFilename2;
		std::wstring LayerMapFilename3;
		std::wstring LayerMapFilename4;
		std::wstring BlendMapFilename;
		float HeightScale;
		UINT HeightmapWidth;
		UINT HeightmapHeight;
		float CellSpacing;
	};

	Terrain(void);
	~Terrain(void);

	float GetWidth()const;
	float GetDepth()const;
	float GetHeight(float x, float z)const;

	XMMATRIX GetWorld()const;
	void SetWorld(CXMMATRIX M);

	void Init(ID3D11Device* device, ID3D11DeviceContext* context, const InitInfo& initInfo);

	void Draw(ID3D11DeviceContext* context, Camera* cam, DirectionalLight lights[3]);

private:
	void LoadHeightmap();
	void Smooth();
	bool InBounds(int i, int j);
	float Average(int i, int j);
	void CalcAllPatchBoundsY();
	void CalcPatchBoundsY(UINT i, UINT j);
	void BuildQuadPatchVB(ID3D11Device* device);
	void BuildQuadPatchIB(ID3D11Device* device);
	void BuildHeightmapSRV(ID3D11Device* device);

	// Divide heightmap into patches such that each patch has CellsPerPatch cells
	// and CellsPerPatch+1 vertices.  Use 64 so that if we tessellate all the way 
	// to 64, we use all the data from the heightmap.  
	static const int CellsPerPatch = 64;

	ID3D11Buffer* VB;
	ID3D11Buffer* IB;

	ID3D11ShaderResourceView* layerTexArraySRV;
	ID3D11ShaderResourceView* blendMapSRV;
	ID3D11ShaderResourceView* heightMapSRV;

	InitInfo info;

	UINT numPatchVertices;
	UINT numPatchQuadFaces;

	UINT numPatchVertRows;
	UINT numPatchVertCols;

	XMFLOAT4X4 world;

	Material mat;

	std::vector<XMFLOAT2> patchBoundsY;
	std::vector<float> heightmap;
};

#endif // TERRAIN_H
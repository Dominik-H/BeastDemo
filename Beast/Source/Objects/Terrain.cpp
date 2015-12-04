/*************************************************************************************
* File: Init.h; Date of creation: 13.Feb.2015; Date of Last Modification: 13.Feb.2015
*
* This is a header file to helper stuff.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#include "Terrain.h"
#include "../Effects.h"
#include "../Vertex.h"
#include <fstream>
#include <sstream>

Terrain::Terrain( void ) : 
	VB( 0 ), 
	IB( 0 ), 
	layerTexArraySRV( 0 ), 
	blendMapSRV( 0 ), 
	heightMapSRV( 0 ),
	numPatchVertices( 0 ),
	numPatchQuadFaces( 0 ),
	numPatchVertRows( 0 ),
	numPatchVertCols( 0 )
{
	XMStoreFloat4x4( &world, XMMatrixIdentity() );

	mat.Ambient  = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
	mat.Diffuse  = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
	mat.Specular = XMFLOAT4( 0.0f, 0.0f, 0.0f, 64.0f );
	mat.Reflect  = XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );
}


Terrain::~Terrain( void )
{
	RELEASE( VB );
	RELEASE( IB );
	RELEASE( layerTexArraySRV );
	RELEASE( blendMapSRV );
	RELEASE( heightMapSRV );
}

float Terrain::GetWidth() const
{
	// Total terrain width.
	return ( info.HeightmapWidth - 1 )*info.CellSpacing;
}

float Terrain::GetDepth() const
{
	// Total terrain depth.
	return ( info.HeightmapHeight - 1 )*info.CellSpacing;
}

float Terrain::GetHeight( float x, float z ) const
{
	// Transform from terrain local space to "cell" space.
	float c = ( x + 0.5f*GetWidth() ) /  info.CellSpacing;
	float d = ( z - 0.5f*GetDepth() ) / -info.CellSpacing;

	// Get the row and column we are in.
	int row = ( int )floorf( d );
	int col = ( int )floorf( c );

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = heightmap[row*info.HeightmapWidth + col];
	float B = heightmap[row*info.HeightmapWidth + col + 1];
	float C = heightmap[(row+1)*info.HeightmapWidth + col];
	float D = heightmap[(row+1)*info.HeightmapWidth + col + 1];

	// Where we are relative to the cell.
	float s = c - ( float )col;
	float t = d - ( float )row;

	// If upper triangle ABC.
	if( s + t <= 1.0f )
	{
		float uy = B - A;
		float vy = C - A;
		return A + s*uy + t*vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + ( 1.0f - s )*uy + ( 1.0f - t )*vy;
	}
}

XMMATRIX Terrain::GetWorld() const
{
	return XMLoadFloat4x4(&world);
}

void Terrain::SetWorld( CXMMATRIX M )
{
	XMStoreFloat4x4( &world, M );
}

void Terrain::Init( ID3D11Device* device, ID3D11DeviceContext* context, const InitInfo& initInfo )
{
	info = initInfo;

	// Divide heightmap into patches such that each patch has CellsPerPatch.
	numPatchVertRows = ( ( info.HeightmapHeight-1 ) / CellsPerPatch ) + 1;
	numPatchVertCols = ( ( info.HeightmapWidth-1 ) / CellsPerPatch ) + 1;

	numPatchVertices  = numPatchVertRows*numPatchVertCols;
	numPatchQuadFaces = ( numPatchVertRows-1 )*( numPatchVertCols-1 );

	LoadHeightmap();
	Smooth();
	CalcAllPatchBoundsY();

	BuildQuadPatchVB( device );
	BuildQuadPatchIB( device );
	BuildHeightmapSRV( device );

	std::vector<std::wstring> layerFilenames;
	layerFilenames.push_back(info.LayerMapFilename0);
	layerFilenames.push_back(info.LayerMapFilename1);
	layerFilenames.push_back(info.LayerMapFilename2);
	layerFilenames.push_back(info.LayerMapFilename3);
	layerFilenames.push_back(info.LayerMapFilename4);
	layerTexArraySRV = d3dHelper::CreateTexture2DArraySRV(device, context, layerFilenames);

	HR(D3DX11CreateShaderResourceViewFromFile(device, 
		info.BlendMapFilename.c_str(), 0, 0, &blendMapSRV, 0));
}

void Terrain::Draw(ID3D11DeviceContext* context, Camera* cam, DirectionalLight lights[3])
{
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	context->IASetInputLayout(InputLayouts::Terrain);

	UINT stride = sizeof(Vertex::Terrain);
  UINT offset = 0;
  context->IASetVertexBuffers(0, 1, &VB, &stride, &offset);
	context->IASetIndexBuffer(IB, DXGI_FORMAT_R16_UINT, 0);

	XMMATRIX viewProj = cam->ViewProj();
	XMMATRIX wworld  = XMLoadFloat4x4(&world);
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(wworld);
	XMMATRIX worldViewProj = wworld*viewProj;

	XMFLOAT4 worldPlanes[6];
	ExtractFrustumPlanes(worldPlanes, viewProj);

	// Set per frame constants.
	Effects::TerrainFX->SetViewProj(viewProj);
	Effects::TerrainFX->SetEyePosW(cam->GetPosition());
	Effects::TerrainFX->SetDirLights(lights);
	Effects::TerrainFX->SetFogColor(Colors::Silver);
	Effects::TerrainFX->SetFogStart(15.0f);
	Effects::TerrainFX->SetFogRange(175.0f);
	Effects::TerrainFX->SetMinDist(20.0f);
	Effects::TerrainFX->SetMaxDist(500.0f);
	Effects::TerrainFX->SetMinTess(0.0f);
	Effects::TerrainFX->SetMaxTess(6.0f);
	Effects::TerrainFX->SetTexelCellSpaceU(1.0f / info.HeightmapWidth);
	Effects::TerrainFX->SetTexelCellSpaceV(1.0f / info.HeightmapHeight);
	Effects::TerrainFX->SetWorldCellSpace(info.CellSpacing);
	Effects::TerrainFX->SetWorldFrustumPlanes(worldPlanes);
	
	Effects::TerrainFX->SetLayerMapArray(layerTexArraySRV);
	Effects::TerrainFX->SetBlendMap(blendMapSRV);
	Effects::TerrainFX->SetHeightMap(heightMapSRV);

	Effects::TerrainFX->SetMaterial(mat);

	ID3DX11EffectTechnique* tech = Effects::TerrainFX->Light1Tech;
    D3DX11_TECHNIQUE_DESC techDesc;
    tech->GetDesc( &techDesc );

    for(UINT i = 0; i < techDesc.Passes; ++i)
    {
        ID3DX11EffectPass* pass = tech->GetPassByIndex(i);
		pass->Apply(0, context);

		context->DrawIndexed(numPatchQuadFaces*4, 0, 0);
	}	

	// FX sets tessellation stages, but it does not disable them.  So do that here
	// to turn off tessellation.
	context->HSSetShader(0, 0, 0);
	context->DSSetShader(0, 0, 0);
}

void Terrain::LoadHeightmap()
{
	// A height for each vertex
	std::vector<unsigned char> in( info.HeightmapWidth * info.HeightmapHeight );

	// Open the file.
	std::ifstream inFile;
	inFile.open(info.HeightMapFilename.c_str(), std::ios_base::binary);

	if(inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		// Done with file.
		inFile.close();
	}

	// Copy the array data into a float array and scale it.
	heightmap.resize(info.HeightmapHeight * info.HeightmapWidth, 0);
	for(UINT i = 0; i < info.HeightmapHeight * info.HeightmapWidth; ++i)
	{
		heightmap[i] = (in[i] / 255.0f)*info.HeightScale;
	}
}

void Terrain::Smooth()
{
	std::vector<float> dest( heightmap.size() );

	for(UINT i = 0; i < info.HeightmapHeight; ++i)
	{
		for(UINT j = 0; j < info.HeightmapWidth; ++j)
		{
			dest[i*info.HeightmapWidth+j] = Average(i,j);
		}
	}

	// Replace the old heightmap with the filtered one.
	heightmap = dest;
}

bool Terrain::InBounds(int i, int j)
{
	// True if ij are valid indices; false otherwise.
	return 
		i >= 0 && i < (int)info.HeightmapHeight && 
		j >= 0 && j < (int)info.HeightmapWidth;
}

float Terrain::Average(int i, int j)
{
	// Function computes the average height of the ij element.
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |ij| 6|
	// ----------
	// | 7| 8| 9|
	// ----------

	float avg = 0.0f;
	float num = 0.0f;

	// Use int to allow negatives.  If we use UINT, @ i=0, m=i-1=UINT_MAX
	// and no iterations of the outer for loop occur.
	for(int m = i-1; m <= i+1; ++m)
	{
		for(int n = j-1; n <= j+1; ++n)
		{
			if( InBounds(m,n) )
			{
				avg += heightmap[m*info.HeightmapWidth + n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

void Terrain::CalcAllPatchBoundsY()
{
	patchBoundsY.resize(numPatchQuadFaces);

	// For each patch
	for(UINT i = 0; i < numPatchVertRows-1; ++i)
	{
		for(UINT j = 0; j < numPatchVertCols-1; ++j)
		{
			CalcPatchBoundsY(i, j);
		}
	}
}

void Terrain::CalcPatchBoundsY(UINT i, UINT j)
{
	// Scan the heightmap values this patch covers and compute the min/max height.

	UINT x0 = j*CellsPerPatch;
	UINT x1 = (j+1)*CellsPerPatch;

	UINT y0 = i*CellsPerPatch;
	UINT y1 = (i+1)*CellsPerPatch;

	float minY = +MathHelper::Infinity;
	float maxY = -MathHelper::Infinity;
	for(UINT y = y0; y <= y1; ++y)
	{
		for(UINT x = x0; x <= x1; ++x)
		{
			UINT k = y*info.HeightmapWidth + x;
			minY = MathHelper::Min(minY, heightmap[k]);
			maxY = MathHelper::Max(maxY, heightmap[k]);
		}
	}

	UINT patchID = i*(numPatchVertCols-1)+j;
	patchBoundsY[patchID] = XMFLOAT2(minY, maxY);
}

void Terrain::BuildQuadPatchVB(ID3D11Device* device)
{
	std::vector<Vertex::Terrain> patchVertices(numPatchVertRows*numPatchVertCols);

	float halfWidth = 0.5f*GetWidth();
	float halfDepth = 0.5f*GetDepth();

	float patchWidth = GetWidth() / (numPatchVertCols-1);
	float patchDepth = GetDepth() / (numPatchVertRows-1);
	float du = 1.0f / (numPatchVertCols-1);
	float dv = 1.0f / (numPatchVertRows-1);

	for(UINT i = 0; i < numPatchVertRows; ++i)
	{
		float z = halfDepth - i*patchDepth;
		for(UINT j = 0; j < numPatchVertCols; ++j)
		{
			float x = -halfWidth + j*patchWidth;

			patchVertices[i*numPatchVertCols+j].Pos = XMFLOAT3(x, 0.0f, z);

			// Stretch texture over grid.
			patchVertices[i*numPatchVertCols+j].Tex.x = j*du;
			patchVertices[i*numPatchVertCols+j].Tex.y = i*dv;
		}
	}

	// Store axis-aligned bounding box y-bounds in upper-left patch corner.
	for(UINT i = 0; i < numPatchVertRows-1; ++i)
	{
		for(UINT j = 0; j < numPatchVertCols-1; ++j)
		{
			UINT patchID = i*(numPatchVertCols-1)+j;
			patchVertices[i*numPatchVertCols+j].BoundsY = patchBoundsY[patchID];
		}
	}

  D3D11_BUFFER_DESC vbd;
  vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::Terrain) * patchVertices.size();
  vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vbd.CPUAccessFlags = 0;
  vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
  vinitData.pSysMem = &patchVertices[0];
  HR(device->CreateBuffer(&vbd, &vinitData, &VB));
}

void Terrain::BuildQuadPatchIB(ID3D11Device* device)
{
	std::vector<USHORT> indices(numPatchQuadFaces*4); // 4 indices per quad face

	// Iterate over each quad and compute indices.
	int k = 0;
	for(UINT i = 0; i < numPatchVertRows-1; ++i)
	{
		for(UINT j = 0; j < numPatchVertCols-1; ++j)
		{
			// Top row of 2x2 quad patch
			indices[k]   = i*numPatchVertCols+j;
			indices[k+1] = i*numPatchVertCols+j+1;

			// Bottom row of 2x2 quad patch
			indices[k+2] = (i+1)*numPatchVertCols+j;
			indices[k+3] = (i+1)*numPatchVertCols+j+1;

			k += 4; // next quad
		}
	}

	D3D11_BUFFER_DESC ibd;
  ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(USHORT) * indices.size();
  ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
  ibd.CPUAccessFlags = 0;
  ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

  D3D11_SUBRESOURCE_DATA iinitData;
  iinitData.pSysMem = &indices[0];
  HR(device->CreateBuffer(&ibd, &iinitData, &IB));
}

void Terrain::BuildHeightmapSRV(ID3D11Device* device)
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = info.HeightmapWidth;
	texDesc.Height = info.HeightmapHeight;
  texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format    = DXGI_FORMAT_R16_FLOAT;
	texDesc.SampleDesc.Count   = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	// HALF is defined in xnamath.h, for storing 16-bit float.
	std::vector<HALF> hmap(heightmap.size());
	std::transform(heightmap.begin(), heightmap.end(), hmap.begin(), XMConvertFloatToHalf);
	
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &hmap[0];
  data.SysMemPitch = info.HeightmapWidth*sizeof(HALF);
  data.SysMemSlicePitch = 0;

	ID3D11Texture2D* hmapTex = 0;
	HR(device->CreateTexture2D(&texDesc, &data, &hmapTex));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	HR(device->CreateShaderResourceView(hmapTex, &srvDesc, &heightMapSRV));

	// SRV saves reference.
	RELEASE(hmapTex);
}
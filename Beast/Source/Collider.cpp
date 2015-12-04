#include "Collider.h"


Collider::Collider()
{
}

Collider::~Collider()
{
#if !defined( DEBUG ) || !defined( _DEBUG )
	for (UINT i = 0; i < OBBs.size(); ++i)
	{
		RELEASE(OBBs[i].second.IB);
		RELEASE(OBBs[i].second.IB);
	}
#endif
}

void CollisionBox::CreateBuffers(ID3D11Device *device)
{
	std::vector<Vertex::Basic> verts;

	for (UINT i = 0; i < 8; ++i)
		verts.push_back(Vertex::Basic());

	float w2 = OBB.Center.x + OBB.Extents.x;
	float h2 = OBB.Center.y + OBB.Extents.y;
	float d2 = OBB.Center.z + OBB.Extents.z;

	const float extent = 0.0f;

	// Fill in the front face vertex data.
	verts[0] = Vertex::Basic(-w2 - extent, -h2 - extent, -d2 - extent, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	verts[1] = Vertex::Basic(-w2 - extent, +h2 + extent, -d2 - extent, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	verts[2] = Vertex::Basic(+w2 + extent, +h2 + extent, -d2 - extent, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	verts[3] = Vertex::Basic(+w2 + extent, -h2 - extent, -d2 - extent, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	verts[4] = Vertex::Basic(-w2 - extent, -h2 - extent, +d2 + extent, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	verts[5] = Vertex::Basic(+w2 + extent, -h2 - extent, +d2 + extent, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	verts[6] = Vertex::Basic(+w2 + extent, +h2 + extent, +d2 + extent, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	verts[7] = Vertex::Basic(-w2 - extent, +h2 + extent, +d2 + extent, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	/*
	// Fill in the top face vertex data.
	verts[8] = Vertex::Basic(-w2 - extent, +h2 + extent, -d2 - extent, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	verts[9] = Vertex::Basic(-w2 - extent, +h2 + extent, +d2 + extent, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	verts[10] = Vertex::Basic(+w2 + extent, +h2 + extent, +d2 + extent, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	verts[11] = Vertex::Basic(+w2 + extent, +h2 + extent, -d2 - extent, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	verts[12] = Vertex::Basic(-w2 - extent, -h2 - extent, -d2 - extent, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	verts[13] = Vertex::Basic(+w2 + extent, -h2 - extent, -d2 - extent, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	verts[14] = Vertex::Basic(+w2 + extent, -h2 - extent, +d2 + extent, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	verts[15] = Vertex::Basic(-w2 - extent, -h2 - extent, +d2 + extent, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	verts[16] = Vertex::Basic(-w2 - extent, -h2 - extent, +d2 + extent, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	verts[17] = Vertex::Basic(-w2 - extent, +h2 + extent, +d2 + extent, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	verts[18] = Vertex::Basic(-w2 - extent, +h2 + extent, -d2 - extent, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	verts[19] = Vertex::Basic(-w2 - extent, -h2 - extent, -d2 - extent, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	verts[20] = Vertex::Basic(+w2 + extent, -h2 - extent, -d2 - extent, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	verts[21] = Vertex::Basic(+w2 + extent, +h2 + extent, -d2 - extent, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	verts[22] = Vertex::Basic(+w2 + extent, +h2 + extent, +d2 + extent, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	verts[23] = Vertex::Basic(+w2 + extent, -h2 - extent, +d2 + extent, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	*/
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::Basic) * verts.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &verts[0];
	HR(device->CreateBuffer(&vbd, &vinitData, &VB));

	//
	// Create the indices.
	//

	UINT i[24];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; 
	i[2] = 1; i[3] = 2; 
	i[4] = 2; i[5] = 3;
	i[6] = 3; i[7] = 0;

	// Fill in the back face index data
	i[8] = 4; i[9] = 5; 
	i[10] = 5; i[11] = 6;
	i[12] = 6; i[13] = 7;
	i[14] = 7; i[15] = 4;

	// Fill in the rest face index data
	i[16] = 0; i[17] = 4;
	i[18] = 1; i[19] = 7;
	i[20] = 2; i[21] = 6;
	i[22] = 3; i[23] = 5;	  

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 36;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &i[0];
	HR(device->CreateBuffer(&ibd, &iinitData, &IB));
}

void Collider::CreateStatic(std::vector<Vertex::PosNormalTexTan> &inVert, const std::vector<USHORT> &inInd, const std::vector<MeshGeometry::Subset> &inSubs)
{
	for (UINT i = 0; i < inSubs.size(); ++i)
	{
		CollisionBox tmp;
		OBBs.push_back(std::pair<int, CollisionBox>(i, tmp));
		UINT start = inInd[inSubs[i].FaceStart];
		UINT count = inInd[inSubs[i].FaceCount * 3] - start;

		XNA::ComputeBoundingOrientedBoxFromPoints(&OBBs.back().second.OBB, count, &inVert[start].Pos, sizeof(Vertex::PosNormalTexTan));
	}
}

void Collider::CreateStatic(std::vector<Vertex::Basic> &inVert)
{
	CollisionBox tmp;
	OBBs.push_back(std::pair<int, CollisionBox>(0, tmp));

	XNA::ComputeBoundingOrientedBoxFromPoints(&OBBs.back().second.OBB, inVert.size(), &inVert[0].Pos, sizeof(Vertex::Basic));
}

void Collider::CreateAnimated(std::vector<Vertex::PosNormalTexTanSkinned> &in)
{
	std::vector<std::pair<int, std::vector<Vertex::PosNormalTexTanSkinned>>> sorted;

	for (UINT i = 0; i < in.size(); ++i)
	{
		for (UINT k = 0; k < 4; ++k)
		{
			if (in[i].BoneIndices[k] == 0)
				continue;

			int is = -1;
			for (UINT j = 0; j < sorted.size(); ++j)
			{
				if (in[i].BoneIndices[k] == sorted[j].first)
				{
					is = j;
					break;
				}
			}

			if (is != -1)
			{
				sorted[is].second.push_back(in[i]);
			}
			else
			{
				std::vector<Vertex::PosNormalTexTanSkinned> tmp;
				sorted.push_back(std::pair<int, std::vector<Vertex::PosNormalTexTanSkinned>>(in[i].BoneIndices[k], tmp));
				sorted.back().second.push_back(in[i]);
			}
		}
	}

	for (UINT i = 0; i < sorted.size(); ++i)
	{
		CollisionBox tmp;
		OBBs.push_back(std::pair<int, CollisionBox>(sorted[i].first, tmp));

		XNA::ComputeBoundingOrientedBoxFromPoints(&OBBs.back().second.OBB, sorted[i].second.size(), &sorted[i].second[0].Pos, sizeof(Vertex::PosNormalTexTanSkinned));
	}
}

void Collider::CreateStaticD(ID3D11Device *device, std::vector<Vertex::PosNormalTexTan> &inVert, const std::vector<USHORT> &inInd, const std::vector<MeshGeometry::Subset> &inSubs)
{
	for (UINT i = 0; i < inSubs.size(); ++i)
	{
		CollisionBox tmp;
		OBBs.push_back(std::pair<int, CollisionBox>(i, tmp));
		UINT start = inInd[inSubs[i].FaceStart];
		UINT count = inInd[inSubs[i].FaceCount * 3] - start;

		XNA::ComputeBoundingOrientedBoxFromPoints(&OBBs.back().second.OBB, count, &inVert[start].Pos, sizeof(Vertex::PosNormalTexTan));

		OBBs.back().second.CreateBuffers(device);
	}
}

void Collider::CreateStaticD(ID3D11Device *device, std::vector<Vertex::Basic> &inVert)
{
	CollisionBox tmp;
	OBBs.push_back(std::pair<int, CollisionBox>(0, tmp));

	XNA::ComputeBoundingOrientedBoxFromPoints(&OBBs.back().second.OBB, inVert.size(), &inVert[0].Pos, sizeof(Vertex::Basic));

	OBBs.back().second.CreateBuffers(device);
}

void Collider::CreateAnimatedD(ID3D11Device *device, std::vector<Vertex::PosNormalTexTanSkinned> &in)
{
	std::vector<std::pair<int, std::vector<Vertex::PosNormalTexTanSkinned>>> sorted;

	for (UINT i = 0; i < in.size(); ++i)
	{
		for (UINT k = 0; k < 4; ++k)
		{
			if (in[i].BoneIndices[k] == 0)
				continue;

			int is = -1;
			for (UINT j = 0; j < sorted.size(); ++j)
			{
				if (in[i].BoneIndices[k] == sorted[j].first)
				{
					is = j;
					break;
				}
			}

			if (is != -1)
			{
				sorted[is].second.push_back(in[i]);
			}
			else
			{
				std::vector<Vertex::PosNormalTexTanSkinned> tmp;
				sorted.push_back(std::pair<int, std::vector<Vertex::PosNormalTexTanSkinned>>(in[i].BoneIndices[k], tmp));
				sorted.back().second.push_back(in[i]);
			}
		}
	}

	for (UINT i = 0; i < sorted.size(); ++i)
	{
		CollisionBox tmp;
		OBBs.push_back(std::pair<int, CollisionBox>(sorted[i].first, tmp));

		XNA::ComputeBoundingOrientedBoxFromPoints(&OBBs.back().second.OBB, sorted[i].second.size(), &sorted[i].second[0].Pos, sizeof(Vertex::PosNormalTexTanSkinned));

		OBBs.back().second.CreateBuffers(device);
	}
}

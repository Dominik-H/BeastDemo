#pragma once
#include "Meshes/MeshGeometry.h"
#include "Meshes/SkinnedData.h"
#include "Meshes/SkinnedModel.h"
#include "Meshes/BasicModel.h"
#include <assimp\Importer.hpp>
#include <assimp\types.h>
#include <assimp/scene.h> 
#include <assimp/postprocess.h>

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	HRESULT LoadModelSkinned(std::string& filename,
		std::vector<Vertex::PosNormalTexTanSkinned>& vertices,
		std::vector<USHORT>& indices,
		std::vector<MeshGeometry::Subset>& subsets,
		std::vector<ExtendedMaterial>& mats,
		SceneAnimator& SkinnedData);

	HRESULT LoadModelStaticBump(std::string& filename,
		std::vector<Vertex::PosNormalTexTan>& vertices,
		std::vector<USHORT>& indices,
		std::vector<MeshGeometry::Subset>& subsets,
		std::vector<ExtendedMaterial>& mats);

	HRESULT LoadModelStaticSimp(std::string& filename,
		std::vector<Vertex::Basic>& vertices,
		std::vector<USHORT>& indices,
		std::vector<MeshGeometry::Subset>& subsets,
		std::vector<ExtendedMaterial>& mats);

	HRESULT LoadAnimation(std::string& filename, SceneAnimator& SkinnedData);

private:
	const aiScene* modelScene;

	std::string* filename; 
	std::vector<Vertex::PosNormalTexTan>* StaticVerticesBump;
	std::vector<Vertex::Basic>* StaticVerticesSimp;
	std::vector<Vertex::PosNormalTexTanSkinned>* SkinnedVertices;
	std::vector<USHORT>* Indices;
	std::vector<MeshGeometry::Subset>* Subsets;
	std::vector<ExtendedMaterial>* Mats;
	SceneAnimator* SkinnedData;

	std::vector<int>* mBoneHierarchy;
	std::vector<std::string>* mBoneNameHierarchy;
	std::vector<XMFLOAT4X4>* mBoneOffsets;

	HRESULT ParseMeshSkinned();
	HRESULT ParseMeshStatic(bool bump);
	HRESULT ParseMaterials();
	HRESULT ParseAnimationData();
	HRESULT ParseBoneHierarchy(aiNode *node);

	XMFLOAT4X4 &aiMatrixToFloat4x4(aiMatrix4x4t<float>& in);
	aiNodeAnim *FindAnimNode(aiAnimation *anim, std::string& nodeName);

	int GetBoneIndex(std::string &boneName)
	{
		for (UINT i = 0; i < mBoneNameHierarchy->size(); ++i)
		{
			if ((*mBoneNameHierarchy)[i] == boneName)
				return i;
		}

		return -1;
	}
};


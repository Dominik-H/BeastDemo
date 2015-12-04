#include "ModelLoader.h"


ModelLoader::ModelLoader()
	//:modelScene(0)
{
}

ModelLoader::~ModelLoader()
{
	filename = nullptr;
	StaticVerticesBump = nullptr;
	StaticVerticesSimp = nullptr;
	SkinnedVertices = nullptr;
	Indices = nullptr;
	Subsets = nullptr;
	Mats = nullptr;
	SkinnedData = nullptr;

	mBoneHierarchy = nullptr;
	mBoneNameHierarchy = nullptr;
	mBoneOffsets = nullptr;

	modelScene = nullptr;
}

HRESULT ModelLoader::LoadModelSkinned(std::string& filename,
									  std::vector<Vertex::PosNormalTexTanSkinned>& vertices,
									  std::vector<USHORT>& indices,
									  std::vector<MeshGeometry::Subset>& subsets,
									  std::vector<ExtendedMaterial>& mats,
									  SceneAnimator& SkinnedData)
{
	Assimp::Importer importer;
	modelScene = importer.ReadFile(filename,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		//aiProcess_MakeLeftHanded |
		aiProcess_FlipWindingOrder |
		aiProcess_FlipUVs |
		aiProcess_FixInfacingNormals |
		aiProcess_GenNormals |
		aiProcess_LimitBoneWeights |
		aiProcess_GenUVCoords);

	// If the import failed, report it
	if (!modelScene)
	{
		return E_FAIL;
	}

	this->filename = &filename;
	this->SkinnedVertices = &vertices;
	this->Indices = &indices;
	this->Subsets = &subsets;
	this->Mats = &mats;
	this->SkinnedData = &SkinnedData;

	SkinnedData.Init(modelScene);

	ParseMeshSkinned();
	ParseMaterials();

	return S_OK;
}

HRESULT ModelLoader::LoadModelStaticBump(std::string& filename,
									 std::vector<Vertex::PosNormalTexTan>& vertices,
									 std::vector<USHORT>& indices,
									 std::vector<MeshGeometry::Subset>& subsets,
									 std::vector<ExtendedMaterial>& mats)
{
	Assimp::Importer importer;
	modelScene = importer.ReadFile(filename,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		//aiProcess_MakeLeftHanded |
		aiProcess_FlipWindingOrder |
		aiProcess_FlipUVs |
		aiProcess_FixInfacingNormals |
		aiProcess_GenNormals |
		aiProcess_LimitBoneWeights |
		aiProcess_GenUVCoords);

	// If the import failed, report it
	if (!modelScene)
	{
		return E_FAIL;
	}

	this->filename = &filename;
	this->StaticVerticesBump = &vertices;
	this->Indices = &indices;
	this->Subsets = &subsets;
	this->Mats = &mats;

	ParseMeshStatic(true);
	ParseMaterials();

	return S_OK;
}

HRESULT ModelLoader::LoadModelStaticSimp(std::string& filename,
	std::vector<Vertex::Basic>& vertices,
	std::vector<USHORT>& indices,
	std::vector<MeshGeometry::Subset>& subsets,
	std::vector<ExtendedMaterial>& mats)
{
	Assimp::Importer importer;
	modelScene = importer.ReadFile(filename,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		//aiProcess_MakeLeftHanded |
		aiProcess_FlipWindingOrder |
		aiProcess_FlipUVs |
		aiProcess_FixInfacingNormals |
		aiProcess_GenNormals |
		aiProcess_LimitBoneWeights);

	// If the import failed, report it
	if (!modelScene)
	{
		return E_FAIL;
	}

	this->filename = &filename;
	this->StaticVerticesSimp = &vertices;
	this->Indices = &indices;
	this->Subsets = &subsets;
	this->Mats = &mats;

	ParseMeshStatic(false);
	ParseMaterials();

	return S_OK;
}

HRESULT ModelLoader::LoadAnimation(std::string& filename, SceneAnimator& SkinnedData)
{
	Assimp::Importer importer;
	modelScene = importer.ReadFile(filename,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		//aiProcess_MakeLeftHanded |
		aiProcess_FlipWindingOrder |
		aiProcess_FlipUVs |
		aiProcess_FixInfacingNormals |
		aiProcess_GenNormals |
		aiProcess_LimitBoneWeights |
		aiProcess_GenUVCoords);

	// If the import failed, report it
	if (!modelScene)
	{
		return E_FAIL;
	}

	this->filename = &filename;

	std::vector<std::string> names;

	int idx = filename.find_last_of("\\/");
	if (idx == std::string::npos)
		idx = -1;
	std::string animName = filename.substr(idx + 1, filename.length() - 5 - idx);
	names.push_back(animName);

	SkinnedData.ExtractAnimations(modelScene, names);

	return S_OK;
}

HRESULT ModelLoader::ParseBoneHierarchy(aiNode* node)
{	
	/*std::string NodeName(node->mName.data);

	
	XMMATRIX NodeTransformation(XMLoadFloat4x4(&aiMatrixToFloat4x4(node->mTransformation)));
	XMFLOAT4X4 tmp;
	XMStoreFloat4x4(&tmp, NodeTransformation);

	if (node->mParent == NULL)
	{
		mBoneHierarchy->push_back(-1);
		mBoneNameHierarchy->push_back(NodeName);
		mBoneOffsets->push_back(tmp);
	}
	else
	{
		std::string d(node->mParent->mName.data);
		mBoneHierarchy->push_back(GetBoneIndex(d));
		mBoneNameHierarchy->push_back(NodeName);
		mBoneOffsets->push_back(tmp);
	}
	

	for (UINT i = 0; i < node->mNumChildren; i++) {
		ParseBoneHierarchy(node->mChildren[i]);
	}*/

	return S_OK;
}

HRESULT ModelLoader::ParseMeshSkinned()
{
	/*// bone hierarchy
	mBoneHierarchy = new std::vector<int>();
	mBoneNameHierarchy = new std::vector<std::string>();
	mBoneOffsets = new std::vector<XMFLOAT4X4>();

	ParseBoneHierarchy(modelScene->mRootNode);

	std::map<std::string, AnimationClip>* mAnimations = new std::map<std::string, AnimationClip>();
	SkinnedData->Set(*mBoneHierarchy, *mBoneNameHierarchy, *mBoneOffsets, *mAnimations);

	SafeDelete(mBoneHierarchy);
	SafeDelete(mBoneNameHierarchy);
	SafeDelete(mBoneOffsets);
	SafeDelete(mAnimations);*/

	UINT lastIndex = 0;

	// mesh
	for (UINT i = 0; i < modelScene->mNumMeshes; ++i)
	{
		// vertices
		for (UINT j = 0; j < modelScene->mMeshes[i]->mNumVertices; ++j)
		{
			Vertex::PosNormalTexTanSkinned tmp;

			aiVector3t<float> v = modelScene->mMeshes[i]->mVertices[j];
			tmp.Pos.x = v.x;
			tmp.Pos.y = v.y;
			tmp.Pos.z = v.z;

			tmp.Normal.x = modelScene->mMeshes[i]->mNormals[j].x;
			tmp.Normal.y = modelScene->mMeshes[i]->mNormals[j].y;
			tmp.Normal.z = modelScene->mMeshes[i]->mNormals[j].z;

			tmp.Tex.x = modelScene->mMeshes[i]->mTextureCoords[0][j].x;
			tmp.Tex.y = modelScene->mMeshes[i]->mTextureCoords[0][j].y;

			tmp.TangentU.x = modelScene->mMeshes[i]->mTangents[j].x;
			tmp.TangentU.y = modelScene->mMeshes[i]->mTangents[j].y;
			tmp.TangentU.z = modelScene->mMeshes[i]->mTangents[j].z;

			XMFLOAT3 norm = tmp.Normal;
			XMFLOAT3 tan;
			tan.x = tmp.TangentU.x;
			tan.y = tmp.TangentU.y;
			tan.z = tmp.TangentU.z;
			XMFLOAT3 biTan;
			biTan.x = modelScene->mMeshes[i]->mBitangents[j].x;
			biTan.y = modelScene->mMeshes[i]->mBitangents[j].y;
			biTan.z = modelScene->mMeshes[i]->mBitangents[j].z;
			XMFLOAT3 w;
			XMStoreFloat3(&w, XMVector3Dot(XMVector3Cross(XMLoadFloat3(&norm), XMLoadFloat3(&tan)), XMLoadFloat3(&biTan)));

			tmp.TangentU.w = (w.x < 0.0f) ? -1.0f : 1.0f;

			tmp.Weights.x = 0.0f;
			tmp.Weights.y = 0.0f;
			tmp.Weights.z = 0.0f;

			tmp.BoneIndices[0] = 0;
			tmp.BoneIndices[1] = 0;
			tmp.BoneIndices[2] = 0;
			tmp.BoneIndices[3] = 0;

			SkinnedVertices->push_back(tmp);
		}

		// bone IDs + weights
		for (UINT j = 0; j < modelScene->mMeshes[i]->mNumBones; ++j) {
			UINT BoneIndex = 0;
			std::string BoneName(modelScene->mMeshes[i]->mBones[j]->mName.data);

			if ((BoneIndex = SkinnedData->GetBoneIndex(BoneName)) == -1) {
				continue;
			}

			/*XMMATRIX t = XMLoadFloat4x4(&SkinnedData->GetBoneOffsets()[BoneIndex]);
			t *= XMLoadFloat4x4(&aiMatrixToFloat4x4(modelScene->mMeshes[i]->mBones[j]->mOffsetMatrix));
			XMStoreFloat4x4(&SkinnedData->GetBoneOffsets()[BoneIndex], t);*/

			for (UINT k = 0; k < modelScene->mMeshes[i]->mBones[j]->mNumWeights; ++k) {

				if (SkinnedVertices->at(lastIndex + modelScene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId).Weights.x == 0.0f)
				{
					SkinnedVertices->at(lastIndex + modelScene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId).BoneIndices[0] = BoneIndex;
					SkinnedVertices->at(lastIndex + modelScene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId).Weights.x = modelScene->mMeshes[i]->mBones[j]->mWeights[k].mWeight;
				}
				else if (SkinnedVertices->at(lastIndex + modelScene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId).Weights.y == 0.0f)
				{
					SkinnedVertices->at(lastIndex + modelScene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId).BoneIndices[1] = BoneIndex;
					SkinnedVertices->at(lastIndex + modelScene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId).Weights.y = modelScene->mMeshes[i]->mBones[j]->mWeights[k].mWeight;
				}
				else if (SkinnedVertices->at(lastIndex + modelScene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId).Weights.z == 0.0f)
				{
					SkinnedVertices->at(lastIndex + modelScene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId).BoneIndices[2] = BoneIndex;
					SkinnedVertices->at(lastIndex + modelScene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId).Weights.z = modelScene->mMeshes[i]->mBones[j]->mWeights[k].mWeight;
				}
				else
				{
					SkinnedVertices->at(lastIndex + modelScene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId).BoneIndices[3] = BoneIndex;
				}
			}
		}

		// indices (faces) + subsets + material ID

		int count = modelScene->mMeshes[i]->mNumFaces;
		MeshGeometry::Subset sub;
		if (Subsets->empty())
			sub.FaceStart = lastIndex;
		else
			sub.FaceStart = Subsets->back().FaceStart + Subsets->back().FaceCount;
		sub.FaceCount = count;
		sub.Id = modelScene->mMeshes[i]->mMaterialIndex;
		Subsets->push_back(sub);

		for (UINT j = 0; j < modelScene->mMeshes[i]->mNumFaces; ++j)
		{
			for (UINT k = 0; k < modelScene->mMeshes[i]->mFaces[j].mNumIndices; ++k)
			{
				if (Subsets->size() == 1)
					Indices->push_back(modelScene->mMeshes[i]->mFaces[j].mIndices[k]);
				else
					Indices->push_back(modelScene->mMeshes[i]->mFaces[j].mIndices[k] + lastIndex);
			}
		}

		
		lastIndex = SkinnedVertices->size();
	}

	return S_OK;	
}

HRESULT ModelLoader::ParseMeshStatic(bool bump)
{
	UINT lastIndex = 0;

	for (UINT i = 0; i < modelScene->mNumMeshes; ++i)
	{
		// vertices
		for (UINT j = 0; j < modelScene->mMeshes[i]->mNumVertices; ++j)
		{
			if (bump)
			{
				Vertex::PosNormalTexTan tmp;

				tmp.Pos.x = modelScene->mMeshes[i]->mVertices[j].x;
				tmp.Pos.y = modelScene->mMeshes[i]->mVertices[j].y;
				tmp.Pos.z = modelScene->mMeshes[i]->mVertices[j].z;

				tmp.Normal.x = modelScene->mMeshes[i]->mNormals[j].x;
				tmp.Normal.y = modelScene->mMeshes[i]->mNormals[j].y;
				tmp.Normal.z = modelScene->mMeshes[i]->mNormals[j].z;

				tmp.Tex.x = modelScene->mMeshes[i]->mTextureCoords[0][j].x;
				tmp.Tex.y = modelScene->mMeshes[i]->mTextureCoords[0][j].y;

				tmp.TangentU.x = modelScene->mMeshes[i]->mTangents[j].x;
				tmp.TangentU.y = modelScene->mMeshes[i]->mTangents[j].y;
				tmp.TangentU.z = modelScene->mMeshes[i]->mTangents[j].z;

				XMFLOAT3 norm = tmp.Normal;
				XMFLOAT3 tan;
				tan.x = tmp.TangentU.x;
				tan.y = tmp.TangentU.y;
				tan.z = tmp.TangentU.z;
				XMFLOAT3 biTan;
				biTan.x = modelScene->mMeshes[i]->mBitangents[j].x;
				biTan.y = modelScene->mMeshes[i]->mBitangents[j].y;
				biTan.z = modelScene->mMeshes[i]->mBitangents[j].z;
				XMFLOAT3 w;
				XMStoreFloat3(&w, XMVector3Dot(XMVector3Cross(XMLoadFloat3(&norm), XMLoadFloat3(&tan)), XMLoadFloat3(&biTan)));

				tmp.TangentU.w = (w.x < 0.0f) ? -1.0f : 1.0f;

				StaticVerticesBump->push_back(tmp);
			}
			else
			{
				Vertex::Basic tmp;

				tmp.Pos.x = modelScene->mMeshes[i]->mVertices[j].x;
				tmp.Pos.y = modelScene->mMeshes[i]->mVertices[j].y;
				tmp.Pos.z = modelScene->mMeshes[i]->mVertices[j].z;

				tmp.Normal.x = modelScene->mMeshes[i]->mNormals[j].x;
				tmp.Normal.y = modelScene->mMeshes[i]->mNormals[j].y;
				tmp.Normal.z = modelScene->mMeshes[i]->mNormals[j].z;

				tmp.Tex.x = modelScene->mMeshes[i]->mTextureCoords[0][j].x;
				tmp.Tex.y = modelScene->mMeshes[i]->mTextureCoords[0][j].y;

				StaticVerticesSimp->push_back(tmp);
			}
		}

		// indices (faces) + subsets + material ID

		int count = modelScene->mMeshes[i]->mNumFaces;
		MeshGeometry::Subset sub;
		if(Subsets->empty())
			sub.FaceStart = lastIndex;
		else
			sub.FaceStart = Subsets->back().FaceStart + Subsets->back().FaceCount;
		sub.FaceCount = count;
		sub.Id = modelScene->mMeshes[i]->mMaterialIndex;
		Subsets->push_back(sub);

		for (UINT j = 0; j < modelScene->mMeshes[i]->mNumFaces; ++j)
		{
			for (UINT k = 0; k < modelScene->mMeshes[i]->mFaces[j].mNumIndices; ++k)
			{
				if (Subsets->size() == 1)
					Indices->push_back(modelScene->mMeshes[i]->mFaces[j].mIndices[k]);
				else
					Indices->push_back(modelScene->mMeshes[i]->mFaces[j].mIndices[k] + lastIndex);
			}
		}

		if (bump)
			lastIndex = StaticVerticesBump->size();
		else
			lastIndex = StaticVerticesSimp->size();		
	}

	return S_OK;
}

HRESULT ModelLoader::ParseMaterials()
{
	for (UINT i = 0; i < modelScene->mNumMaterials; ++i)
	{
		ExtendedMaterial tmpMat;
		for (UINT j = 0; j < modelScene->mMaterials[i]->mNumProperties; ++j)
		{
			aiColor3D color(0.0f, 0.0f, 0.0f);
			float opacity = 1.0f;
			modelScene->mMaterials[i]->Get(AI_MATKEY_OPACITY, opacity);

			// Ambient
			modelScene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, color);
			tmpMat.Ambient.x = color.r;
			tmpMat.Ambient.y = color.g;
			tmpMat.Ambient.z = color.b;
			tmpMat.Ambient.w = opacity;

			// Specular
			color = aiColor3D(0.0f, 0.0f, 0.0f);
			float pow = 0.0f;
			modelScene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, color);
			modelScene->mMaterials[i]->Get(AI_MATKEY_SHININESS, pow);
			tmpMat.Specular.x = color.r;
			tmpMat.Specular.y = color.g;
			tmpMat.Specular.z = color.b;
			tmpMat.Specular.w = pow;

			// Diffuse
			color = aiColor3D(0.0f, 0.0f, 0.0f);
			modelScene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			tmpMat.Diffuse.x = color.r;
			tmpMat.Diffuse.y = color.g;
			tmpMat.Diffuse.z = color.b;
			tmpMat.Diffuse.w = opacity;

			// Reflect
			color = aiColor3D(0.0f, 0.0f, 0.0f);
			modelScene->mMaterials[i]->Get(AI_MATKEY_COLOR_REFLECTIVE, color);
			tmpMat.Reflect.x = color.r;
			tmpMat.Reflect.y = color.g;
			tmpMat.Reflect.z = color.b;
			tmpMat.Reflect.w = opacity;

			if (modelScene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) >= 1)
			{
				aiString path;
				modelScene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path);

				tmpMat.diffuseMapName = path.C_Str();
			}

			if (modelScene->mMaterials[i]->GetTextureCount(aiTextureType_NORMALS) >= 1)
			{
				aiString path;
				modelScene->mMaterials[i]->GetTexture(aiTextureType_NORMALS, 0, &path);

				tmpMat.normalMapName = path.C_Str();
			}
		}

		Mats->push_back(tmpMat);
	}
	return S_OK;
}

HRESULT ModelLoader::ParseAnimationData()
{
	/*std::map<std::string, AnimationClip> animClips = SkinnedData->getAnimations();
	int idx = filename->find_last_of("\\/");
	if (idx == std::string::npos)
		idx = -1;
	std::string animName = filename->substr(idx + 1, filename->length() - 5 - idx);
	AnimationClip clip;

	// process clip
	aiNodeAnim *animNode;
	for (UINT i = 0; i < SkinnedData->BoneCount(); ++i)
	{
		animNode = FindAnimNode(modelScene->mAnimations[0], SkinnedData->GetBoneName(i));

		if (animNode == NULL)
		{
			BoneAnimation boneAnim;
			for (UINT j = 0; j < 3; ++j)
			{
				Keyframe key;
				key.TimePos = 0.0f;

				key.Translation.x = 0.0f;
				key.Translation.y = 0.0f;
				key.Translation.z = 0.0f;

				key.Scale.x = 0.0f;
				key.Scale.y = 0.0f;
				key.Scale.z = 0.0f;

				key.RotationQuat.x = 0.0f;
				key.RotationQuat.y = 0.0f;
				key.RotationQuat.z = 0.0f;
				key.RotationQuat.w = 0.0f;

				boneAnim.Keyframes.push_back(key);
			}

			clip.BoneAnimations.push_back(boneAnim);
			continue;
		}

		BoneAnimation boneAnim;
		for (UINT j = 0; j < animNode->mNumPositionKeys; ++j)
		{
			Keyframe key;
			key.TimePos = animNode->mPositionKeys[j].mTime / modelScene->mAnimations[0]->mTicksPerSecond;

			key.Translation.x = animNode->mPositionKeys[j].mValue.x;
			key.Translation.y = animNode->mPositionKeys[j].mValue.y;
			key.Translation.z = animNode->mPositionKeys[j].mValue.z;

			key.Scale.x = animNode->mScalingKeys[j].mValue.x;
			key.Scale.y = animNode->mScalingKeys[j].mValue.y;
			key.Scale.z = animNode->mScalingKeys[j].mValue.z;
			
			key.RotationQuat.x = animNode->mRotationKeys[j].mValue.x;
			key.RotationQuat.y = animNode->mRotationKeys[j].mValue.y;
			key.RotationQuat.z = animNode->mRotationKeys[j].mValue.z;
			key.RotationQuat.w = animNode->mRotationKeys[j].mValue.w;

			boneAnim.Keyframes.push_back(key);
		}

		clip.BoneAnimations.push_back(boneAnim);
	}	

	animClips.insert(std::pair<std::string, AnimationClip>(animName, clip));
	SkinnedData->setAnimations(animClips);*/

	return S_OK;
}

XMFLOAT4X4 &ModelLoader::aiMatrixToFloat4x4(aiMatrix4x4t<float>& in)
{
	XMFLOAT4X4 tmp;

	tmp._11 = in.a1;
	tmp._12 = in.a2;
	tmp._13 = in.a3;
	tmp._14 = in.a4;

	tmp._21 = in.b1;
	tmp._22 = in.b2;
	tmp._23 = in.b3;
	tmp._24 = in.b4;

	tmp._31 = in.c1;
	tmp._31 = in.c2;
	tmp._33 = in.c3;
	tmp._34 = in.c4;

	tmp._41 = in.d1;
	tmp._42 = in.d2;
	tmp._43 = in.d3;
	tmp._44 = in.d4;

	return tmp;
}

aiNodeAnim *ModelLoader::FindAnimNode(aiAnimation *anim, std::string& nodeName)
{
	for (UINT i = 0; i < modelScene->mAnimations[0]->mNumChannels; ++i)
	{
		std::string t(anim->mChannels[i]->mNodeName.data);
		if (t == nodeName)
		{
			return anim->mChannels[i];
		}
	}

	return NULL;
}

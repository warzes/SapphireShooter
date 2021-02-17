#pragma once

#include "Mesh2.h"

#define NUM_BONES_PER_VEREX 4

class MeshBone : public Mesh2
{
public:
	struct BoneData
	{
		void addBoneData(const unsigned& boneId, const float& weight);

		unsigned ids[NUM_BONES_PER_VEREX];
		float weights[NUM_BONES_PER_VEREX];
	};

private:
	unsigned VBObones;
	std::vector<BoneData> bonesData;

	void initBones();

public:
	MeshBone(const std::vector<MeshVertex>& verts, const std::vector<unsigned>& inds, const std::vector<MeshTexture>& texs, const Material& mat, const std::vector<BoneData>& bones);
	virtual ~MeshBone();

	const std::vector<BoneData>& getVerticesBoneData() const { return bonesData; }
};
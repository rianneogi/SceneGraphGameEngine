#pragma once

#include "SkyBox.h"

class Water
{
public:
	glm::mat4 mModelMat;
	MeshDataTex* mMesh;
	Texture2D* mDuDvTexture;

	float mDuDvOffset;
	float mWaveSpeed;
	float mWaveStrength;
	float mWaveHeight;
	float mSpecularFactor;
	float mShineDamper;

	FrameBuffer mReflectionFBO;
	FrameBuffer mRefractionFBO;

	Water(MeshDataTex* mesh, Texture2D* tex);
	~Water();
	
	void update(int deltaTime);
	void render();
	void bindReflection();
	void bindRefraction();
};


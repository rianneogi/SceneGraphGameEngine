#pragma once

#include "SkyBox.h"

class Water
{
public:
	glm::mat4 mModelMat;
	Mesh* mMesh;
	float mDuDvOffset;
	float mWaveSpeed;
	float mWaveStrength;
	float mWaveHeight;
	float mSpecularFactor;
	float mShineDamper;

	FrameBuffer mReflectionFBO;
	FrameBuffer mRefractionFBO;

	Water(Mesh* mesh);
	~Water();

	void update(int deltaTime);
	void render();
	void bindReflection();
	void bindRefraction();
};


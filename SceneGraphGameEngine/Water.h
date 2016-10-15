#pragma once

#include "SkyBox.h"

class Water
{
public:
	glm::mat4 mModelMat;
	Mesh* mMesh;

	FrameBuffer mReflectionFBO;
	FrameBuffer mRefractionFBO;

	Water();
	~Water();

	void render();
	void bindReflection();
	void bindRefraction();
};

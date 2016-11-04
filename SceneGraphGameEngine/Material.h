#pragma once

#include "CubeMapTexture.h"

class Material
{
public:
	Texture* mTexture;
	Texture* mNormalMap;
	Texture* mDisplacementMap;
	Texture* mSpecularMap;
	float mSpecularIntensity;
	float mShineDamper;

	Material();
	Material(Texture* tex, float intensity, float damper);
	~Material();
};


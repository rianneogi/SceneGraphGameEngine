#pragma once

#include "TextureArray.h"

//Contains texture data for a 6 sided cube
class CubeMapTexture : public Texture
{
public:
	CubeMapTexture();
	~CubeMapTexture();

	bool load(const std::string& PosXFilename,const std::string& NegXFilename,const std::string& PosYFilename,
		const std::string& NegYFilename,const std::string& PosZFilename,const std::string& NegZFilename);
	bool loadRepeated(const std::string& Filename);
	void bind();
	void bind(GLenum TextureUnit);
	void freeTexture();

	GLuint mTextureID;
};


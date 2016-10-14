#pragma once

#include "Texture.h"

class Texture2DArray
{
public:
	Texture2DArray();
	~Texture2DArray();

	bool loadFromFile(std::vector<std::string> paths);
	bool loadTextureFromPixels32(GLuint* pixels, GLuint width, GLuint height);
	void freeTexture();
	void bind(); //binds texture to GL_TEXTURE0
	void bind(GLenum textureunit);
	void unbind();

	//sf::Texture mTex;
	GLuint mTextureID;
	unsigned int mTextureWidth;
	unsigned int mTextureHeight;
	unsigned int mTextureDepth;
};

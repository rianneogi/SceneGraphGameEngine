#pragma once

#include "Texture.h"

class TextureArray : public Texture
{
public:
	TextureArray();
	~TextureArray();

	bool loadFromFile(std::vector<Texture2D*> textures, unsigned int width, unsigned int height, unsigned int bpt);
	void freeTexture();
	void bind(); //binds texture to GL_TEXTURE0
	void bind(GLenum textureunit);
	void unbind();

	GLuint mTextureID;
	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mDepth;
	unsigned int mBytesPerTexel;
};

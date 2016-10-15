#pragma once

#include "GraphicsInclude.h"

///Wrapper class for sf::Texture

class Texture
{
public:
	Texture();
	Texture(std::string file);
	~Texture();

	bool loadFromFile(std::string path);
	bool loadTextureFromPixels32(GLuint* pixels, GLuint width, GLuint height);
	void freeTexture();
	void bind(); //binds texture to GL_TEXTURE0
	void bind(GLenum textureunit);
	void unbind();

	GLuint mTextureID;
	unsigned int mTextureWidth;
	unsigned int mTextureHeight;
};




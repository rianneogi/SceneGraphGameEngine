#pragma once

#include "Timer.h"

///Wrapper class for sf::Texture

class Texture
{
public:
	virtual void bind() = 0;
	virtual void bind(GLenum textureunit) = 0;
};

class Texture2D : public Texture
{
public:
	Texture2D();
	Texture2D(std::string file);
	~Texture2D();

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




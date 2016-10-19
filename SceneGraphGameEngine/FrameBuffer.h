#pragma once

#include "CubeMapTexture.h"

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

class FrameBuffer
{
public:
	GLuint mFBO;
	GLuint mColorTexture;
	GLuint mDepthTexture;
	GLuint mDepthBuffer;

	unsigned int mWidth;
	unsigned int mHeight;

	FrameBuffer();
	~FrameBuffer();

	void bind();
	void unbind();

	void createFBO(int width, int height);
	void createColorTextureAttachment();
	void createDepthTextureAttachment();
	void createDepthBufferAttachment();

	void freeColorTexture();
	void freeDepthTexture();
	void freeDepthBuffer();
	void freeFBO();
	bool checkStatus() const;
};


#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
{
	mFBO = 0;
	mDepthBuffer = 0;
	mDepthTexture = 0;
	mColorTexture = 0;
	mWidth = 0;
	mHeight = 0;
}

FrameBuffer::~FrameBuffer()
{
	freeFBO();
	freeDepthBuffer();
	freeColorTexture();
	freeDepthTexture();
}

void FrameBuffer::bind()
{
	glBindTexture(GL_TEXTURE_2D, NULL);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glViewport(0, 0, mWidth, mHeight);
}

void FrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void FrameBuffer::createFBO(int width, int height)
{
	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	debugOpengl("fbo init error");
}

void FrameBuffer::createColorTextureAttachment()
{
	GLuint attachment = 0;
	glGenTextures(1, &attachment);
	glBindTexture(GL_TEXTURE_2D, attachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, attachment, 0);
}

void FrameBuffer::createDepthTextureAttachment()
{
	GLuint attachment = 0;
	glGenTextures(1, &attachment);
	glBindTexture(GL_TEXTURE_2D, attachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, attachment, 0);
}

void FrameBuffer::createDepthBufferAttachment()
{
	GLuint attachment = 0;
	glGenRenderbuffers(1, &attachment);
	glBindRenderbuffer(GL_RENDERBUFFER, attachment);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, attachment);
}

void FrameBuffer::freeColorTexture()
{
	if (mColorTexture != NULL)
	{
		glDeleteTextures(1, &mColorTexture);
	}
}

void FrameBuffer::freeDepthTexture()
{
	if (mDepthTexture != NULL)
	{
		glDeleteTextures(1, &mDepthTexture);
	}
}

void FrameBuffer::freeDepthBuffer()
{
	if (mDepthBuffer != NULL)
	{
		glDeleteRenderbuffers(1, &mDepthBuffer);
	}
}

void FrameBuffer::freeFBO()
{
	if (mFBO != NULL)
	{
		glDeleteFramebuffers(1, &mFBO);
	}
}

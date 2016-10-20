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
	printf("freeing FBO %d\n", mFBO);
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

void FrameBuffer::initFBO(int width, int height, GLenum drawbuffer)
{
	mWidth = width;
	mHeight = height;

	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glDrawBuffer(drawbuffer);
	//glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	debugOpengl("fbo init error");

	printf("Created FBO %d\n", mFBO);
}

void FrameBuffer::createColorTextureAttachment()
{
	freeColorTexture();

	//glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	glGenTextures(1, &mColorTexture);
	glBindTexture(GL_TEXTURE_2D, mColorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTexture, 0);
}

void FrameBuffer::createDepthTextureAttachment()
{
	freeDepthTexture();

	//glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	glGenTextures(1, &mDepthTexture);
	glBindTexture(GL_TEXTURE_2D, mDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, mWidth, mHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);
}

void FrameBuffer::createDepthBufferAttachment()
{
	freeDepthBuffer();

	//glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	glGenRenderbuffers(1, &mDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer);
}

void FrameBuffer::freeColorTexture()
{
	if (mColorTexture != NULL)
	{
		printf("Deleting color texture %d\n", mFBO);
		glDeleteTextures(1, &mColorTexture);
	}
}

void FrameBuffer::freeDepthTexture()
{
	if (mDepthTexture != NULL)
	{
		printf("Deleting depth texture %d\n", mFBO);
		glDeleteTextures(1, &mDepthTexture);
	}
}

void FrameBuffer::freeDepthBuffer()
{
	if (mDepthBuffer != NULL)
	{
		printf("Deleting render buffer %d\n", mFBO);
		glDeleteRenderbuffers(1, &mDepthBuffer);
	}
}

void FrameBuffer::freeFBO()
{
	if (mFBO != NULL)
	{
		printf("Deleting framebuffer %d\n", mFBO);
		glDeleteFramebuffers(1, &mFBO);
	}
}

bool FrameBuffer::checkStatus() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	GLenum status = (GLenum)glCheckFramebufferStatus(GL_FRAMEBUFFER);

	switch (status)
	{

	case GL_FRAMEBUFFER_UNSUPPORTED:
		printf("OpenGL framebuffer format not supported.\n");
		return false;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		printf("OpenGL framebuffer missing attachment.\n");
		return false;
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		printf("OpenGL framebuffer attached images must have same dimensions.\n");
		return false;
	//case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
	//	printf("OpenGL framebuffer attached images must have same format.\n");
	//	return false;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		printf("OpenGL framebuffer missing draw buffer.\n");
		return false;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		printf("OpenGL framebuffer missing read buffer.\n");
		return false;
	case GL_FRAMEBUFFER_COMPLETE_EXT:
	default:
		return true;
	}

	return true;
}
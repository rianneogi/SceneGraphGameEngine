#include "TextureArray.h"

TextureArray::TextureArray()
{
	mHeight = 0;
	mWidth = 0;
	mDepth = 0;
	mBytesPerTexel = 0;
}

TextureArray::~TextureArray()
{
	freeTexture();
}

bool TextureArray::loadFromFile(std::vector<Texture*> textures, unsigned int width, unsigned int height, unsigned int bpt)
{
	mHeight = height;
	mWidth = width;
	mDepth = textures.size();
	mBytesPerTexel = bpt;

	GLenum err = GL_NO_ERROR;
	//typedef char BYTE;
	//allocate memory for the texels
	//typedef unsigned char BYTE;
	GLubyte* texels = (GLubyte*)malloc(mWidth * mHeight * textures.size() * mBytesPerTexel);
	for (size_t i = 0;i < textures.size();i++)
	{
		//sf::Image img = gTextures[i].mTex.copyToImage();
		textures[i]->bind();
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &texels[mBytesPerTexel*mWidth*mHeight*i]);
		//glReadPixels(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, &texels[BYTES_PER_TEXEL*TEXTURE_WIDTH*TEXTURE_HEIGHT*i]);
		//for (int j = 0;j < TEXTURE_HEIGHT;j++)
		//{
		//	for (int k = 0;k < TEXTURE_WIDTH;k++)
		//	{
		//		//sf::Color col = img.getPixel(j, k);
		//		//printf("pixel %d %d %d %d\n", col.r, col.g, col.b, col.a);
		//		texels[BYTES_PER_TEXEL*(TEXTURE_WIDTH*TEXTURE_HEIGHT*i + TEXTURE_WIDTH*j + k)] = 255;
		//		texels[BYTES_PER_TEXEL*(TEXTURE_WIDTH*TEXTURE_HEIGHT*i + TEXTURE_WIDTH*j + k) + 1] = 0;
		//		texels[BYTES_PER_TEXEL*(TEXTURE_WIDTH*TEXTURE_HEIGHT*i + TEXTURE_WIDTH*j + k) + 2] = 255;
		//		texels[BYTES_PER_TEXEL*(TEXTURE_WIDTH*TEXTURE_HEIGHT*i + TEXTURE_WIDTH*j + k) + 3] = 127;
		//	}
		//}
	}

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, mTextureID);

	debugOpengl("texture array generation start");
	//glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA, TEXTURE_WIDTH, TEXTURE_HEIGHT, TEXTURE_DEPTH);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_SRGB_ALPHA, mWidth, mHeight, textures.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	//glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, TEXTURE_WIDTH, TEXTURE_HEIGHT, TEXTURE_DEPTH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels);
	for (int i = 0;i < textures.size();i++)
	{
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, mWidth, mHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, &texels[mBytesPerTexel*mWidth*mHeight*i]);
	}
	
	//Parameters
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_LOD_BIAS, 0);
	debugOpengl("Texture Array paramaters");

	//Mipmaps
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	//Anisotropic Filtering
	if (GLEW_EXT_texture_filter_anisotropic)
	{
		float amount = 0;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &amount);
		if (amount > 4)
		{
			amount = 4;
		}
		glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY_EXT, amount);
	}
	else
	{
		printf("texture filter anisotropic not supported\n");
	}
	//glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	free(texels);

	debugOpengl("texture array generation end");

	return true;
}

void TextureArray::freeTexture()
{
	//Delete texture
	if (mTextureID != 0)
	{
		printf("freeing texture: %d\n", mTextureID);
		glDeleteTextures(1, &mTextureID);
		mTextureID = 0;
	}

	mWidth = 0;
	mHeight = 0;
	mDepth = 0;
	mBytesPerTexel = 0;
}

void TextureArray::bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, mTextureID);
}

void TextureArray::bind(GLenum textureunit)
{
	glActiveTexture(textureunit);
	glBindTexture(GL_TEXTURE_2D_ARRAY, mTextureID);
}

void TextureArray::unbind()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, NULL);
}

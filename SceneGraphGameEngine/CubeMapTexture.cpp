#include "CubeMapTexture.h"

CubeMapTexture::CubeMapTexture()
{
	mTextureID = 0;
}

CubeMapTexture::~CubeMapTexture()
{
	freeTexture();
}

void CubeMapTexture::freeTexture()
{
	if (mTextureID != 0)
	{
		glDeleteTextures(1, &mTextureID);
	}
}

bool CubeMapTexture::load(const std::string& PosXFilename,
	const std::string& NegXFilename,
	const std::string& PosYFilename,
	const std::string& NegYFilename,
	const std::string& PosZFilename,
	const std::string& NegZFilename)
{
	freeTexture();

	std::string* fileNames = new std::string[6];
	fileNames[0] = PosXFilename;
	fileNames[1] = NegXFilename;
	fileNames[2] = PosYFilename;
	fileNames[3] = NegYFilename;
	fileNames[4] = PosZFilename;
	fileNames[5] = NegZFilename;

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);

	for (int i = 0;i < 6;i++)
	{
		ILuint imgID = 0;
		ilGenImages(1, &imgID);
		ilBindImage(imgID);

		ILboolean success = ilLoadImage(fileNames[i].c_str());

		if (success == IL_TRUE)
		{
			//Convert image to RGBA
			success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

			if (success == IL_TRUE)
			{
				//Create texture from file pixels
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, (GLuint)ilGetInteger(IL_IMAGE_WIDTH), (GLuint)ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLuint*)ilGetData());
			}

			//Delete file from memory
			ilDeleteImages(1, &imgID);
		}
		else
		{
			printf("Error loading: %s", fileNames[i].c_str());
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	delete[] fileNames;

	return true;
}

bool CubeMapTexture::loadRepeated(const std::string& Filename)
{
	return load(Filename, Filename, Filename, Filename, Filename, Filename);
}

void CubeMapTexture::bind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);
}

void CubeMapTexture::bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);
}

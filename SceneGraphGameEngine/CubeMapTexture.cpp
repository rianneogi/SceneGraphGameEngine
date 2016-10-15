#include "CubeMapTexture.h"

CubeMapTexture::CubeMapTexture()
{
	mTextureObj = 0;
}

CubeMapTexture::~CubeMapTexture()
{
}

bool CubeMapTexture::load(const std::string& PosXFilename,
	const std::string& NegXFilename,
	const std::string& PosYFilename,
	const std::string& NegYFilename,
	const std::string& PosZFilename,
	const std::string& NegZFilename)
{
	std::string* fileNames = new std::string[6];
	fileNames[0] = PosXFilename;
	fileNames[1] = NegXFilename;
	fileNames[2] = PosYFilename;
	fileNames[3] = NegYFilename;
	fileNames[4] = PosZFilename;
	fileNames[5] = NegZFilename;

	glGenTextures(1, &mTextureObj);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureObj);

	/*for (int i = 0;i < 6;i++)
	{
		sf::Image t;
		t.loadFromFile(fileNames[i]);
		sf::Vector2u size = t.getSize();
		glTexImage2D(i, 0, GL_RGB, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, t.getPixelsPtr());
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}*/

	return true;
}

bool CubeMapTexture::loadRepeated(const std::string& Filename)
{
	return load(Filename, Filename, Filename, Filename, Filename, Filename);
}

void CubeMapTexture::bind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureObj);
}

void CubeMapTexture::bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureObj);
}

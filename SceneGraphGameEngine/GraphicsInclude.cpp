#include "GraphicsInclude.h"

bool debugOpengl(std::string errorstring)
{
	GLenum error = GL_NO_ERROR;
	bool outputOK = true;
	while ((error = glGetError()) != GL_NO_ERROR)
	{
		printf("!!ERROR!! %s : %d - %s\n", errorstring.c_str(), error, gluErrorString(error));
		outputOK = false;
	}
	return outputOK;
}

void printVector(glm::vec3 v)
{
	for (int i = 0; i < 3; i++)
	{
		printf("%f ", v[i]);
	}
	printf("\n");
}

void printMatrix(glm::mat4 m)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%f ", m[i][j]);
		}
		printf("\n");
	}
}

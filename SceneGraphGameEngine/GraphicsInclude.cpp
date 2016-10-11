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
#pragma once

#include <SDL.h>
#include <IL\il.h>
#include <IL\ilu.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <vector>

#define GLM_FORCE_RADIANS

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\noise.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

bool debugOpengl(std::string errorstring);

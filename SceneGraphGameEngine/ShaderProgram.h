#pragma once

#include "Mesh.h"
#include "stdio.h"
#include <string>
#include <fstream>
#include <conio.h>

extern unsigned int gActiveShader;

struct ColorRGBA
{
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;

	ColorRGBA() : r(0), b(0), g(0), a(0) {}
	ColorRGBA(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) : r(red), b(blue), g(green), a(alpha) {}
};

class ShaderProgram
{
public:
	ShaderProgram();
	/*
	Pre Condition:
	-None
	Post Condition:
	-Initializes variables
	Side Effects:
	-None
	*/

	~ShaderProgram();
	/*
	Pre Condition:
	-None
	Post Condition:
	-Frees shader program
	Side Effects:
	-None
	*/

	bool loadProgram(std::string path);
	/*
	Pre Condition:
	-A valid OpenGL context
	Post Condition:
	-Loads shader program
	Side Effects:
	-None
	*/

	void freeProgram();
	/*
	Pre Condition:
	-None
	Post Condition:
	-Frees shader program if it exists
	Side Effects:
	-None
	*/

	bool bind();
	/*
	Pre Condition:
	-A loaded shader program
	Post Condition:
	-Sets this program as the current shader program
	-Reports to console if there was an error
	Side Effects:
	-None
	*/

	void unbind();
	/*
	Pre Condition:
	-None
	Post Condition:
	-Sets default shader program as current program
	Side Effects:
	-None
	*/

	GLuint getProgramID();
	/*
	Pre Condition:
	-None
	Post Condition:
	-Returns program ID
	Side Effects:
	-None
	*/

	void setUniformInt(std::string name, int val);
	void setUniformFloat(std::string name, float val);
	void setUniformMat4f(std::string name, glm::mat4 matrix);
	void setUniformVec3f(std::string name, glm::vec3 vec);
	void setUniformVec4f(std::string name, glm::vec4 vec);

protected:
	void printProgramLog(GLuint program);
	/*
	Pre Condition:
	-None
	Post Condition:
	-Prints program log
	-Reports error is GLuint ID is not a shader program
	Side Effects:
	-None
	*/

	void printShaderLog(GLuint shader);
	/*
	Pre Condition:
	-None
	Post Condition:
	-Prints shader log
	-Reports error is GLuint ID is not a shader
	Side Effects:
	-None
	*/

	GLuint loadShaderFromFile(std::string path, GLenum shaderType);
	/*
	Pre Condition:
	-None
	Post Condition:
	-Returns the ID of a compiled shader of the specified type from the specified file
	-Reports error to console if file could not be found or compiled
	Side Effects:
	-None
	*/

	//Program ID
	GLuint mProgramID;
};


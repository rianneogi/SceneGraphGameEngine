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
	ShaderProgram(std::string path);
	ShaderProgram(std::string path, std::string defs);
	~ShaderProgram();

	bool loadProgram(std::string path, std::string defs = "");
	void freeProgram();
	bool bind();
	void unbind();

	GLuint getProgramID();

	void setUniformInt(const std::string& name, int val);
	void setUniformFloat(const std::string& name, float val);
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void setUniformVec3f(const std::string& name, const glm::vec3& vec);
	void setUniformVec4f(const std::string& name, const glm::vec4& vec);
	void setTextureLocation(const std::string& name, unsigned int location);

protected:
	void printProgramLog(GLuint program);
	void printShaderLog(GLuint shader);

	GLuint loadShaderFromFile(std::string path, GLenum shaderType, std::string defs);

	GLuint mProgramID;
};


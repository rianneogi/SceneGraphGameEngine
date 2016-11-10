#include "ShaderProgram.h"
#include <conio.h>

ShaderProgram::ShaderProgram()
{
	mProgramID = NULL;
}

ShaderProgram::ShaderProgram(std::string path)
{
	mProgramID = NULL;
	loadProgram(path);
}

ShaderProgram::ShaderProgram(std::string path, std::string defs)
{
	mProgramID = NULL;
	loadProgram(path, defs);
}

ShaderProgram::~ShaderProgram()
{
	freeProgram();
}

void ShaderProgram::freeProgram()
{
	if (mProgramID != NULL)
	{
		glDeleteProgram(mProgramID);
		debugOpengl("delete shader error");
	}
}

bool ShaderProgram::bind()
{
	debugOpengl("pre-bind shader");
	assert(mProgramID != NULL);
	glUseProgram(mProgramID);
	return debugOpengl("post-bind shader");
}

void ShaderProgram::unbind()
{
	glUseProgram(NULL);
}

GLuint ShaderProgram::getProgramID()
{
	return mProgramID;
}

void ShaderProgram::printProgramLog(GLuint program)
{
	//Make sure name is shader
	if (glIsProgram(program))
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
			_getch();
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a program\n", program);
		_getch();
	}
}

void ShaderProgram::printShaderLog(GLuint shader)
{
	//Make sure name is shader
	if (glIsShader(shader))
	{
		//Shader log length
		int infoLogLength = 0;

		//Get info string length
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		//Allocate string
		char* infoLog = new char[infoLogLength];

		//Get info log
		int len;
		glGetShaderInfoLog(shader, infoLogLength, &len, infoLog);
		if (len > 0)
		{
			printf("%s\n", infoLog);
			_getch();
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a shader\n", shader);
		_getch();
	}
}

GLuint ShaderProgram::loadShaderFromFile(std::string path, GLenum shaderType, std::string defs)
{
	//Open file
	GLuint shaderID = 0;
	std::string shaderString;
	std::ifstream sourceFile(path.c_str());

	//Source file loaded
	if (sourceFile)
	{
		//Get shader source
		shaderString.assign((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());

		//Create shader ID
		shaderID = glCreateShader(shaderType);

		//Set shader source
		std::string tmp = "#version 330\n" + defs + shaderString;
		const GLchar* shaderSource = tmp.c_str();
		glShaderSource(shaderID, 1, (const GLchar**)&shaderSource, NULL);

		//Compile shader source
		glCompileShader(shaderID);

		//Check shader for errors
		GLint shaderCompiled = GL_FALSE;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
		if (shaderCompiled != GL_TRUE)
		{
			printf("Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, shaderSource);
			printShaderLog(shaderID);
			glDeleteShader(shaderID);
			shaderID = 0;
			_getch();
		}
	}
	else
	{
		printf("Unable to open file %s\n", path.c_str());
		_getch();
	}

	return shaderID;
}

bool ShaderProgram::loadProgram(std::string path, std::string defs)
{
	freeProgram();
	
	//Generate program
	mProgramID = glCreateProgram();

	printf("creating shader program, id: %d, path: %s, defs: %s\n", mProgramID, path.c_str(), defs.c_str());
	

	//Load vertex shader
	GLuint vertexShader = loadShaderFromFile(path+".glvs", GL_VERTEX_SHADER, defs);

	//Check for errors
	if (vertexShader == 0)
	{
		glDeleteProgram(mProgramID);
		mProgramID = 0;
		return false;
	}

	//Attach vertex shader to program
	glAttachShader(mProgramID, vertexShader);


	//Create fragment shader
	GLuint fragmentShader = loadShaderFromFile(path+".glfs", GL_FRAGMENT_SHADER, defs);

	//Check for errors
	if (fragmentShader == 0)
	{
		glDeleteShader(vertexShader);
		glDeleteProgram(mProgramID);
		mProgramID = 0;
		return false;
	}

	//Attach fragment shader to program
	glAttachShader(mProgramID, fragmentShader);

	//Link program
	glLinkProgram(mProgramID);

	//Check for errors
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		printf("Error linking program %d!\n", mProgramID);
		printProgramLog(mProgramID);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(mProgramID);
		mProgramID = 0;
		return false;
	}

	//Clean up excess shader references
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	debugOpengl("Loading shader error");

	return true;
}

void ShaderProgram::setUniformInt(const std::string& name, int val)
{
	glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), val);
}

void ShaderProgram::setUniformFloat(const std::string& name, float val)
{
	glUniform1f(glGetUniformLocation(mProgramID, name.c_str()), val);
}

void ShaderProgram::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
void ShaderProgram::setUniformVec3f(const std::string& name, const glm::vec3& vec)
{
	glUniform3fv(glGetUniformLocation(mProgramID, name.c_str()), 1, glm::value_ptr(vec));
}

void ShaderProgram::setUniformVec4f(const std::string& name, const glm::vec4& vec)
{
	glUniform4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, glm::value_ptr(vec));
}

void ShaderProgram::setTextureLocation(const std::string& name, unsigned int location)
{
	glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), location);
}
